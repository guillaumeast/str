#pragma once

#include "Column.hpp"

class Row
{
public:

    Text merged;
    string column_separator;
    vector<Column> columns;
    vector<Text> lines;
    size_t height = 0;
    size_t visible_width = 0;

    // --- CONSTRUCTORS ---

    Row() = default;
    Row(const vector<Column> &cols, const string &separator);

    // --- METHODS ---

    
private:

    void _expand_columns(void);

    void _merge_lines(const string &separator);
};

