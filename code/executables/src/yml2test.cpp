/*
 * yml2test.cpp
 *
 *  Created on: Feb 19, 2015
 *      Author: cady
 */

#include <fstream>
#include <iostream>
#include <string>

void remove_carriage_return(std::string& line);
void remove_carriage_return(std::string& line)
{
    if (*line.rbegin() == '\r')
    {
        line.erase(line.length() - 1);
    }
}

std::string remove_file_extension(const std::string& filename);
std::string remove_file_extension(const std::string& filename)
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <YAML file>" << std::endl;
    }
    std::ifstream file(argv[1]);

    const std::string function_name = remove_file_extension(argv[1]);

    if (not(file.is_open()))
    {
        std::cout << "Unable to open file '" << argv[1] << "'" << std::endl;
        return -1;
    }

    std::string line;
    bool first_line = true;
    std::cout << "std::string test_data::" << function_name << "()" << std::endl;
    std::cout << "{" << std::endl;
    std::cout << "    std::stringstream ss;";
    while (std::getline(file, line))
    {
        remove_carriage_return(line);
        std::cout << std::endl;
        if (first_line) std::cout << "    ss << ";
        else            std::cout << "       << ";
        std::cout << "\"" << line << "\\n\"";
        first_line = false;
    }
    std::cout << ";" << std::endl << "    return ss.str();" << std::endl
              << "}" << std::endl;
    return 0;
}


