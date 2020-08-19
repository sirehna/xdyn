/*
 * generate_stl_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */


#include "file_writer.hpp"
#include "generate_test_ship.hpp"
#include "hdb_data.hpp"
#include "stl_data.hpp"
#include "stl_writer.hpp"
#include "TriMeshTestData.hpp"
#include <google/protobuf/stubs/common.h>
#include <fstream>

int main(int argc, const char* argv[])
{
    const std::string path = argc>1 ? std::string(argv[1])+"/" : "";
    std::map<std::string,std::string> stl;

    stl["single_facet.stl"] = test_data::single_facet();
    stl["three_facets.stl"] = test_data::three_facets();
    stl["cube.stl"]         = test_data::cube();
    stl["big_cube.stl"]     = test_data::big_cube();
    stl["L.stl"]            = write_stl(L());
    stl["U.stl"]            = write_stl(U());
    stl["test_ship.stl"]    = write_stl(test_ship());
    stl["test_ship.hdb"]    = test_data::test_ship_hdb();

    write_files(path, stl);
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
