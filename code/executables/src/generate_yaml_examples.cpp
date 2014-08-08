/*
 * generate_yaml_examples.cpp
 *
 *  Created on: 17 avr. 2014
 *      Author: cady
 */

#include <fstream>

#include "yaml_data.hpp"

int main(int argc, const char* argv[])
{
    const std::string path = argc>1 ? std::string(argv[1])+"/" : "";
    std::ofstream file2(path+"hydrostatic_integration_test.yml"),
                  file3(path+"falling_ball_test.yml"),
                  file4(path+"oscillating_cube_test.yml"),
                  file5(path+"stable_cube_test.yml"),
                  file6(path+"stable_rolling_cube_test.yml"),
                  file7(path+"anthineas_equilibre.yml"),
                  file8(path+"postprocessing.yml");

    file2 << test_data::hydrostatic_test();
    file2.close();

    file3 << test_data::falling_ball_example();
    file3.close();

    file4 << test_data::oscillating_cube_example();
    file4.close();

    file5 << test_data::stable_cube_example();
    file5.close();

    file6 << test_data::stable_rolling_cube_test();
    file6.close();

    file7 << test_data::anthineas_hydrostatic_test();
    file7.close();

    file8 << test_data::outputs();
    file8.close();
    return 0;
}
