/*
 * generate_yaml_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <fstream>

#include "file_writer.hpp"
#include "generate_anthineas.hpp"
#include "STL_data.hpp"
#include "stl_writer.hpp"

int main(int argc, const char* argv[])
{
    const std::string path = argc>1 ? std::string(argv[1])+"/" : "";
    std::map<std::string,std::string> stl;

    stl["single_facet.stl"] = test_data::single_facet();
    stl["three_facets.stl"] = test_data::three_facets();
    stl["cube.stl"]         = test_data::cube();
    stl["big_cube.stl"]     = test_data::big_cube();
    stl["anthineas.stl"]    = write_stl(anthineas());

    write_files(path, stl);
    return 0;
}
