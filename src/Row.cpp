#include "Row.hpp"

// --- CONSTRUCTORS ---

Row::Row(const vector<Column> &cols, const string &separator)
{
    columns = cols;
    column_separator = separator;
    
    _expand_columns();
    _merge_lines(separator);

    merged = Text::join(lines, "\n");
}

// --- PRIVATE ---

void Row::_expand_columns(void)
{
    height = 0;
    for (const Column &col : columns)
    {
        if (col.height > height) height = col.height;
    }

    for (Column &col : columns)
    {
        col.set_height(height);
    }
}

void Row::_merge_lines(const string &separator)
{
    lines.clear();
    visible_width = 0;
    for (size_t i = 0; i < height; i++)
    {
        string tmp_line = columns[0].lines[i].raw;
        for (size_t j = 1; j < columns.size(); j++)
        {
            tmp_line += separator + columns[j].lines[i].raw;
        }

        Text line = Text(tmp_line);
        if (line.visible_width > visible_width)
        {
            visible_width = line.visible_width;
        }

        lines.push_back(line);
    }
}

