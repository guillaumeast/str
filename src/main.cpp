#include <string>
#include <iostream>
#include <cstdlib>      // std::atoi
#include "Row.hpp"

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <command> <utf8_string>\n";
        return 1;
    }

    const string command = argv[1];
    if (command == "width")
    {
        Column column(argv[2]);
        std::cout << column.visible_width << "\n";
    }
    else if (command == "height")
    {
        Column column(argv[2]);
        std::cout << column.height << "\n";
    }
    else if (command == "clean")
    {
        Text text(argv[2]);
        std::cout << text.visible << "\n";
    }
    else if (command == "split")
    {
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " split <string> <separator>\n";
            return 1;
        }

        vector<string> parts = Text::split(argv[2], argv[3]);
        for (const auto& part : parts) {
            std::cout << part << "\n";
        }
    }
    else if (command == "repeat")
    {
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " repeat <count> <string> [separator]\n";
            return 1;
        }

        size_t count = std::atoi(argv[2]);
        string separator = (argc >= 5) ? argv[4] : "";

        string repeated = Text::repeat(count, argv[3], separator);
        std::cout << repeated << "\n";
    }
    else if (command == "row")
    {
        if (argc < 5)
        {
            std::cerr << "Usage: " << argv[0] << "row <separator> <string1> <string2> [...]\n";
            return 1;
        }

        string separator = argv[2];
        vector<Column> columns;

        for (int i = 3; i < argc; i++)
        {
            Column column = Column(argv[i]);
            columns.push_back(column);
        }

        std::cout << Row(columns, separator).merged.raw << "\n";
    }
    else
    {
        std::cerr << "Unknown command: " << command << "\n";
        return 1;
    }

    return 0;
}

