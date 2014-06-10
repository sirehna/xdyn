/*
 * generate_yaml_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <fstream>

#include "STL_data.hpp"

int main(int , const char* argv[])
{
    (void) argv;
    std::ofstream file1("single_facet.stl"),
                  file2("three_facets.stl"),
                  file3("cube.stl");

    file1 << test_data::single_facet();
    file2 << test_data::three_facets();
    file3 << test_data::cube();

    return 0;
}
