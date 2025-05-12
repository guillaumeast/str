#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistr.h>
#include <uniwidth.h>
#include <stdbool.h>
#include <stddef.h>

void free_string_array(char **array, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        free(array[i]);
    }
    free(array);
}

// Remove ANSI escape sequences (e.g. colors) from a UTF-8 string
char *clean(const char *utf8_string) {
    size_t len = strlen(utf8_string);
    char *output = malloc(len + 1);
    if (!output) return NULL;

    size_t in = 0, out = 0;
    while (utf8_string[in]) {
        if (utf8_string[in] == '\033' && utf8_string[in + 1] == '[') {
            in += 2;
            while (utf8_string[in] && !(utf8_string[in] >= '@' && utf8_string[in] <= '~')) {
                in++;
            }
            if (utf8_string[in]) in++;  // skip final letter (like 'm')
        } else {
            output[out++] = utf8_string[in++];
        }
    }

    output[out] = '\0';
    return output;
}

// Return the display width of a UTF-8 string
int get_width(const char *utf8_string) {
    char *clean_input = clean(utf8_string);
    if (!clean_input) return -1;

    int max_width = 0;
    const char *line_start = clean_input;

    while (*line_start) {
        const char *line_end = strchr(line_start, '\n');
        size_t line_length = line_end
            ? (size_t)(line_end - line_start)
            : strlen(line_start);

        int width = u8_width((const uint8_t *)line_start, line_length, "UTF-8");
        if (width > max_width) {
            max_width = width;
        }

        if (!line_end) break;
        line_start = line_end + 1;
    }

    free(clean_input);
    return max_width;
}

// Return the number of lines in a UTF-8 string (based on '\n' count + 1)
int get_height(const char *utf8_string) {

    if (!utf8_string || utf8_string[0] == '\0') {
        return 0;
    }

    int lines = 1; // At least one line if non-empty
    for (const char *c = utf8_string; *c != '\0'; ++c) {
        if (*c == '\n') {
            lines++;
        }
    }
    return lines;
}

char **split(const char *input, const char *separator, size_t *count) {
    size_t sep_len = strlen(separator);
    size_t capacity = 8;
    size_t parts = 0;

    char **result = malloc(capacity * sizeof(char *));
    if (!result) return NULL;

    const char *current = input;
    const char *next;

    while ((next = strstr(current, separator)) != NULL) {
        size_t len = next - current;
        char *part = malloc(len + 1);
        if (!part) {
            // clean up
            for (size_t i = 0; i < parts; ++i) free(result[i]);
            free(result);
            return NULL;
        }

        memcpy(part, current, len);
        part[len] = '\0';

        if (parts >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char *));
            if (!new_result) {
                for (size_t i = 0; i < parts; ++i) free(result[i]);
                free(result);
                return NULL;
            }
            result = new_result;
        }

        result[parts++] = part;
        current = next + sep_len;
    }

    // add the final part
    char *final = strdup(current);
    if (!final) {
        for (size_t i = 0; i < parts; ++i) free(result[i]);
        free(result);
        return NULL;
    }
    result[parts++] = final;

    *count = parts;
    return result;
}

char *repeat(int count, const char *string, const char *separator) {
    if (count <= 0 || !string || !separator) return NULL;

    size_t str_len = strlen(string);
    size_t sep_len = strlen(separator);

    // Calculate total length
    size_t total_len = count * str_len + (count - 1) * sep_len + 1;

    char *result = malloc(total_len);
    if (!result) return NULL;

    result[0] = '\0';

    for (int i = 0; i < count; ++i) {
        strcat(result, string);
        if (i < count - 1) {
            strcat(result, separator);
        }
    }

    return result;
}

/*
 * merge_as_row
 * ------------
 * Merge N multiline strings row‑by‑row, inserting `separator` between columns.
 * Every column is padded to its maximum display width (get_width), so all rows
 * end up aligned and share the exact same display width.
 *
 *   string_count – number of columns
 *   strings      – array of input strings
 *   separator    – text inserted between columns
 *
 * Returns a newly allocated string (caller must free) or NULL on OOM.
 */
char *merge_as_row(size_t string_count, const char **strings, const char *separator)
{
    if (string_count == 0) return strdup("");

    /* Split inputs and measure max width per column */
    size_t   *line_counts = calloc(string_count, sizeof(size_t));
    char   ***lines       = calloc(string_count, sizeof(char **));
    int      *col_widths  = calloc(string_count, sizeof(int));
    size_t    max_rows    = 0;

    if (!line_counts || !lines || !col_widths) goto oom;

    for (size_t col = 0; col < string_count; ++col) {
        lines[col] = split(strings[col], "\n", &line_counts[col]);
        if (!lines[col]) goto oom;

        if (line_counts[col] > max_rows)
            max_rows = line_counts[col];

        for (size_t r = 0; r < line_counts[col]; ++r) {
            int w = get_width(lines[col][r]);
            if (w > col_widths[col]) col_widths[col] = w;
        }
    }

    /* Prepare dynamic output buffer */
    size_t sep_len = strlen(separator);
    size_t capacity = 256, length = 0;
    char  *out = malloc(capacity);
    if (!out) goto oom;

#define ENSURE_CAP(N)                                                     \
    do {                                                                  \
        if (length + (N) >= capacity) {                                   \
            while (length + (N) >= capacity) capacity *= 2;               \
            char *tmp = realloc(out, capacity);                           \
            if (!tmp) goto oom;                                           \
            out = tmp;                                                    \
        }                                                                 \
    } while (0)

    /* Build each row */
    for (size_t row = 0; row < max_rows; ++row) {
        for (size_t col = 0; col < string_count; ++col) {

            /* Select cell or generate padding when line missing */
            const char *cell;
            char       *pad_tmp = NULL;

            if (row < line_counts[col]) {
                cell = lines[col][row];
            } else {
                pad_tmp = calloc((size_t)col_widths[col] + 1, 1);
                if (!pad_tmp) goto oom;
                memset(pad_tmp, ' ', (size_t)col_widths[col]);
                cell = pad_tmp;
            }

            /* Copy cell */
            ENSURE_CAP(strlen(cell));
            memcpy(out + length, cell, strlen(cell));
            length += strlen(cell);

            /* Right‑pad cell if narrower than column width */
            int pad_needed = col_widths[col] - get_width(cell);
            if (pad_needed > 0) {
                ENSURE_CAP((size_t)pad_needed);
                memset(out + length, ' ', (size_t)pad_needed);
                length += (size_t)pad_needed;
            }

            if (pad_tmp) free(pad_tmp);

            /* Add separator except after last column */
            if (col < string_count - 1) {
                ENSURE_CAP(sep_len);
                memcpy(out + length, separator, sep_len);
                length += sep_len;
            }
        }

        /* Newline except after last row */
        if (row < max_rows - 1) {
            ENSURE_CAP(1);
            out[length++] = '\n';
        }
    }
    out[length] = '\0';

    /* Cleanup temporaries */
    for (size_t col = 0; col < string_count; ++col)
        free_string_array(lines[col], line_counts[col]);
    free(lines);
    free(line_counts);
    free(col_widths);

    return out;

oom:
    fprintf(stderr, "[merge_as_row] out of memory\n");
    if (lines)
        for (size_t col = 0; col < string_count; ++col)
            if (lines[col]) free_string_array(lines[col], line_counts[col]);
    free(lines);
    free(line_counts);
    free(col_widths);
    free(out);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <command> <utf8_string>\n", argv[0]);
        return 1;
    }

    const char *command = argv[1];
    const char *input = argv[2];

    if (strcmp(command, "width") == 0) {
        int width = get_width(input);
        printf("%d\n", width);
    } else if (strcmp(command, "height") == 0) {
        int height = get_height(input);
        printf("%d\n", height);
    } else if (strcmp(command, "clean") == 0) {
        char *cleaned = clean(input);
        printf("%s\n", cleaned);
    } else if (strcmp(command, "split") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Usage: %s split <string> <separator>\n", argv[0]);
            return 1;
        }
    
        size_t count;
        char **parts = split(argv[2], argv[3], &count);
        if (!parts) {
            fprintf(stderr, "Error: split failed (memory allocation)\n");
            return 1;
        }
    
        for (size_t i = 0; i < count; ++i) {
            printf("%s\n", parts[i]);
        }
    
        free_string_array(parts, count);
    } else if (strcmp(command, "repeat") == 0) {
        if (argc < 5) {
            fprintf(stderr, "Usage: %s repeat <count> <string> <separator>\n", argv[0]);
            return 1;
        }
    
        int count = atoi(argv[2]);
        if (count <= 0) {
            fprintf(stderr, "Error: count must be a positive integer\n");
            return 1;
        }
    
        const char *str = argv[3];
        const char *sep = argv[4];
    
        char *result = repeat(count, str, sep);
        if (!result) {
            fprintf(stderr, "Error: repeat failed (memory allocation)\n");
            return 1;
        }
    
        puts(result);
        free(result);
    } else if (strcmp(command, "row") == 0) {
        if (argc < 5) {
            fprintf(stderr,
                    "Usage: %s row <separator> <string1> <string2> [...]\n",
                    argv[0]);
            return 1;
        }
    
        const char *separator   = argv[2];
        const char **strings    = (const char **)&argv[3];
        size_t string_count     = (size_t)(argc - 3);
    
        char *merged = merge_as_row(string_count, strings, separator);
        if (!merged) return 1;
    
        puts(merged);
        free(merged);
         
    } else {
        fprintf(stderr, "Unknown command: %s\n", command);
        return 1;
    }

    return 0;
}

