#include <unistr.h>
#include <uniwidth.h>
#include "Text.hpp"

static string _clean(const string &input);
static size_t _get_width(const string &input);

// --- CONSTRUCTOR ---

Text::Text(const string &input)
{
    raw = input;
    visible = _clean(input);
    visible_width = _get_width(visible);
}

// --- PUBLIC ---

string Text::join(const vector<string> &strings, const string &separator)
{
    if (strings.empty()) return "";

    string result = strings[0];
    for (size_t i = 1; i < strings.size(); ++i)
    {
        result += separator + strings[i];
    }

    return result;
}

string Text::join(const vector<Text> &texts, const string &separator)
{
    if (texts.empty()) return "";

    vector<string> tmp_strings;
    for (const auto &text : texts)
    {
        tmp_strings.push_back(text.raw);
    }

    return Text::join(tmp_strings, separator);
}

vector<string> Text::split(const string &str, const string &separator)
{
    vector<string> result;
    size_t separator_length = separator.length();
    size_t start = 0;

    while (true) {
        size_t position = str.find(separator, start);
        if (position == string::npos) break;

        result.push_back(str.substr(start, position - start));
        start = position + separator_length;
    }

    result.push_back(str.substr(start));
    return result;
}

string Text::repeat(size_t &count, const string &str, const string &separator)
{
    if (count == 0) return "";

    string result = str;
    for (size_t i = 1; i < count; ++i) {
        result += separator + str;
    }

    return result;
}

// --- PRIVATE ---

// Remove ANSI escape sequences
static string _clean(const string &input)
{
    string output;
    size_t i = 0;

    while (i < input.length()) {
        if (input[i] == '\033' && i + 1 < input.length() && input[i + 1] == '[') {
            i += 2;
            while (i < input.length() && !(input[i] >= '@' && input[i] <= '~')) {
                i++;
            }
            if (i < input.length()) i++;
        } else {
            output += input[i++];
        }
    }

    return output;
}

// Return display width (unicode-aware)
static size_t _get_width(const string &input)
{
    const uint8_t *utf8_cast = reinterpret_cast<const uint8_t *>(input.c_str());

    int width = u8_width(utf8_cast, input.length(), "UTF-8");

    return (width < 0) ? 0 : static_cast<size_t>(width);
}

