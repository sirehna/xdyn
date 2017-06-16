#include "file_writer.hpp"
#include <fstream>

void write_file(const std::string& filename, const std::string& contents)
{
    std::ofstream file(filename);
    file << contents;
}

void write_files(const std::string& path, const std::map<std::string,std::string>& files)
{
    for (auto that_file = files.begin() ; that_file != files.end() ; ++that_file)
    {
        write_file(path + that_file->first, that_file->second);
    }
}
