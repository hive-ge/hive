#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <system_error>
#include <unordered_map>


namespace hive
{
    namespace resource
    {
        /**
         * Methods for loading resources
         */
        std::string loadUTF8File(std::string file_path)
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
    } // namespace resource
} // namespace hive