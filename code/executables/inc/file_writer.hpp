#ifndef FILE_WRITER_HPP_
#define FILE_WRITER_HPP_

#include <map>
#include <string>

void write_file(const std::string& filename, const std::string& contents);
void write_files(const std::string& path, const std::map<std::string,std::string>& files);

#endif  /* FILE_WRITER_HPP_ */
