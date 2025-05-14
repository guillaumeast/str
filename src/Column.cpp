#include "Column.hpp"

// --- CONSTRUCTORS ---

Column::Column(const string &str)
{
    set_content(str);
}

Column::Column(const vector<string> &strings)
{
    _set_content(strings);
}

Column::Column(const Text &text)
{
    set_content(text);
}

Column::Column(const vector<Text> &texts)
{
    set_content(texts);
}

// --- PUBLIC ---

void Column::set_content(const string &str)
{
    vector<string> strings;
    strings.push_back(str);
    _set_content(strings);
}

void Column::set_content(const vector<string> &strings)
{
    _set_content(strings);
}

void Column::set_content(const Text &text)
{
    vector<string> strings;
    strings.push_back(text.raw);
    _set_content(strings);
}

void Column::set_content(const vector<Text> &texts)
{
    vector<string> strings;
    for (const auto &text : texts)
    {
        strings.push_back(text.raw);
    }
    _set_content(strings);
}

void Column::set_height(const size_t value)
{
    vector<Text> new_lines;
    for (size_t i = 0; i < lines.size() && i < value; i++)
    {
        new_lines.push_back(lines[i]);
    }

    size_t missing_height = value - new_lines.size();
    for (size_t i = 0; i < missing_height; i++)
    {
        new_lines.push_back(Text());
    }

    merged = Text::join(lines, "\n");

    _update_size();
}

// --- PRIVATE ---

void Column::_set_content(const vector<string> &strings)
{
    merged = Text::join(strings, "\n");

    vector<string> splitted_strings = Text::split(merged.raw, "\n");

    lines.clear();
    for (const auto &string : splitted_strings)
    {
        lines.push_back(string);
    }

    _update_size();
}

void Column::_update_size(void)
{
    height = lines.size();

    visible_width = 0;
    for (const auto &line : lines)
    {
        if (line.visible_width > visible_width)
        {
            visible_width = line.visible_width;
        }
    }

    for (auto &line : lines)
    {
        if (line.visible_width >= visible_width) continue;

        size_t missing_width = visible_width - line.visible_width;

        line = line.raw + Text::repeat(missing_width, " ");
    }
}

