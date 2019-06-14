/*
 * generate_yaml_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include "file_writer.hpp"
#include "yaml_data.hpp"
#include <fstream>

int main(int argc, const char* argv[])
{
    const std::string path = argc>1 ? std::string(argv[1])+"/" : "";

    std::map<std::string,std::string> yaml;

    yaml["tutorial_01_falling_ball.yml"]      = test_data::falling_ball_example();
    yaml["tutorial_02_fast_hydrostatic.yml"]  = test_data::test_ship_fast_hydrostatic_test();
    yaml["tutorial_02_exact_hydrostatic.yml"] = test_data::test_ship_exact_hydrostatic_test();
    yaml["tutorial_03_waves.yml"]             = test_data::waves();
    yaml["tutorial_04_damping.yml"]           = test_data::test_ship_damping();
    yaml["tutorial_05_froude_krylov.yml"]     = test_data::test_ship_froude_krylov();
    yaml["tutorial_06_propulsion.yml"]        = test_data::test_ship_propulsion() + test_data::controlled_forces();
    yaml["tutorial_06_1D_propulsion.yml"]     = test_data::propulsion_and_resistance() + test_data::controlled_forces();
    yaml["tutorial_07_radiation_damping.yml"] = test_data::test_ship_radiation_damping() + test_data::controlled_forces();
    yaml["tutorial_08_diffraction.yml"]       = test_data::test_ship_diffraction();
    yaml["tutorial_09_gRPC_wave_model.yml"]   = test_data::tutorial_09_gRPC_wave_model();
    yaml["oscillating_cube_test.yml"]         = test_data::oscillating_cube_example();
    yaml["stable_cube_test.yml"]              = test_data::stable_cube_example();
    yaml["stable_rolling_cube_test.yml"]      = test_data::stable_rolling_cube_test();
    yaml["cube_in_waves.yml"]                 = test_data::cube_in_waves();
    yaml["test_ship_in_waves.yml"]            = test_data::test_ship_waves_test();

    write_files(path, yaml);

    return 0;
}
