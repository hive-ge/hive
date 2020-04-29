#pragma once

#include "primitive/typedef.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>
#include <unordered_map>
#include <vector>


namespace hive
{

    /**
     * Methods for loading resources
     */
    inline std::string loadUTF8File(const std::string file_path)
    {
        std::ifstream file;

        std::stringstream str;

        file.open(file_path);

        if (file.good()) {
            char c = file.get();
            while (file.good()) {
                str << c;
                c = file.get();
            }
        }

        file.close();

        return str.str();
    }

    /**
     * Methods for loading resources
     */
    inline std::vector<ubyte> loadCharFile(const std::string file_path)
    {
        std::vector<ubyte> buffer;

        std::ifstream file;

        file.open(file_path);

        while (file.good()) {

            buffer.push_back(file.get());
        }

        file.close();

        return buffer;
    }
} // namespace hive