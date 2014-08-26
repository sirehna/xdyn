/*
 * generate_yaml_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <fstream>
#include "generate_anthineas.hpp"
#include "STL_data.hpp"
#include "stl_writer.hpp"

int main(int argc, const char* argv[])
{
    const std::string path = argc>1 ? std::string(argv[1])+"/" : "";
    std::ofstream file1(path+"single_facet.stl"),
                  file2(path+"three_facets.stl"),
                  file3(path+"cube.stl"),
                  file4(path+"big_cube.stl"),
                  file5(path+"anthineas.stl");

    file1 << test_data::single_facet();
    file2 << test_data::three_facets();
    file3 << test_data::cube();
    file4 << test_data::big_cube();
    file5 << write_stl(anthineas());

    return 0;
}
