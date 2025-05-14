#pragma once

#include "types.hpp"
#include "Text.hpp"

class Text {
public:
    string raw = "";
    string visible = "";
    size_t visible_width = 0;

    // --- CONSTRUCTORS ---

    Text() = default;
    Text(const string &input);

    // --- METHODS ---

    static string repeat(size_t &count, const string &str, const string &separator = "");

    static string join(const vector<string> &strings, const string &separator = "");
    static string join(const vector<Text> &texts, const string &separator = "");

    static vector<string> split(const string &str, const string &separator);
};

