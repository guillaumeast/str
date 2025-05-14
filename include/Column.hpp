#pragma once

#include "Text.hpp"

// Texts are automatically splitted on "\n"
class Column
{
public:

    Text merged;
    vector<Text> lines;
    size_t height = 0;
    size_t visible_width = 0;

    // --- CONSTRUCTORS ---

    Column() = default;
    Column(const string &str);
    Column(const vector<string> &strings);
    Column(const Text &text);
    Column(const vector<Text> &texts);

    // --- METHODS ---

    void set_content(const string &str);
    void set_content(const vector<string> &strings);
    void set_content(const Text &text);
    void set_content(const vector<Text> &texts);

    void set_height(const size_t height);
    
private:

    void _set_content(const vector<string> &strings);

    void _update_size(void);
};

