#include "mesh_manipulations.hpp"
#include "stl_reader.hpp"
#include "stl_writer.hpp"
#include "base91.hpp"
#include <google/protobuf/stubs/common.h>
#include <ssc/text_file_reader.hpp>
#include <iostream>
#include <string>

std::string description();
std::string description()
{
    std::stringstream ss;
    ss << "Takes a STL (STereoLithography) ascii file & transforms it to a C++ source code (written to standard output)."
       << "This is done so as to avoid putting the whole ascii file in a string (unacceptable build time)." << std::endl
       << "(c) SIREHNA 2014-2015." << std::endl
       << std::endl
       << "ID : @GIT_SHA1@" << std::endl
       << std::endl;
    return ss.str();
}

std::string write_cpp(const VectorOfVectorOfPoints& points);
std::string write_cpp(const VectorOfVectorOfPoints& points)
{
    std::stringstream ss;
    ss << "#include \"GeometricTypes3d.hpp\"\n"
       << "\n"
       << "VectorOfVectorOfPoints stl();\n"
       << "VectorOfVectorOfPoints stl()\n"
       << "{\n"
       << "    VectorOfVectorOfPoints ret;\n"
       << "    ret.reserve(" << points.size() << ");\n"
       << "    VectorOfPoints v;\n\n";
    size_t old_size = 0;
    for (size_t i = 0 ; i < points.size() ; ++i)
    {
        if (old_size != points[i].size())
        {
            ss << "    v.resize(" << points[i].size() << ");\n";
            old_size = points[i].size();
        }
        for (size_t j = 0 ; j < points[i].size() ; ++j)
        {
            ss << "    v[" << j << "] = EPoint(" << points[i][j](0) << ","
                                             << points[i][j](1) << ","
                                             << points[i][j](2) << ");\n";
        }
        ss << "    ret.push_back(v);\n\n";
    }
    ss << "    return ret;\n"
       << "}\n";
    return ss.str();
}

std::string write_binary_cpp(const VectorOfVectorOfPoints& points);
std::string write_binary_cpp(const VectorOfVectorOfPoints& points)
{
    std::stringstream ss;
    write_binary_stl(points, ss);
    return base<91>::encode(ss.str());
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << description() << std::endl << "Usage: " << argv[0] << " <stl file>" << std::endl;
        return 1;
    }

    const ssc::text_file_reader::TextFileReader stl_file(std::vector<std::string>(1, std::string(argv[argc-1])));
    const VectorOfVectorOfPoints points = read_stl(stl_file.get_contents());

    std::cout << "const std::string s = \"" << write_binary_cpp(points) << "\";\n";
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
