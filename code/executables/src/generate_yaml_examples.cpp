/*
 * generate_yaml_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <fstream>

#include "file_writer.hpp"
#include "yaml_data.hpp"

int main(int argc, const char* argv[])
{
    const std::string path = argc>1 ? std::string(argv[1])+"/" : "";

    std::map<std::string,std::string> yaml;

    yaml["tutorial_01_falling_ball.yml"]      = test_data::falling_ball_example();
    yaml["tutorial_02_hydrostatic.yml"]       = test_data::hydrostatic_test();
    yaml["tutorial_03_exact_hydrostatic.yml"] = test_data::anthineas_exact_hydrostatic_test();
    yaml["tutorial_04_waves.yml"]             = test_data::waves();
    yaml["oscillating_cube_test.yml"]         = test_data::oscillating_cube_example();
    yaml["stable_cube_test.yml"]              = test_data::stable_cube_example();
    yaml["stable_rolling_cube_test.yml"]      = test_data::stable_rolling_cube_test();
    yaml["fast_hydrostatic_anthineas.yml"]    = test_data::anthineas_fast_hydrostatic_test();
    yaml["postprocessing.yml"]                = test_data::outputs();
    yaml["cube_in_waves.yml"]                 = test_data::cube_in_waves();
    yaml["anthineas_in_waves.yml"]            = test_data::anthineas_waves_test();


    write_files(path, yaml);

    return 0;
}
