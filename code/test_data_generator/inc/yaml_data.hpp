/*
 * yaml_data.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAML_DATA_HPP_
#define YAML_DATA_HPP_

#include <string>

namespace test_data
{
    std::string hydrostatic_test();
    std::string added_mass_from_file();
    std::string full_example();
    std::string full_example_with_propulsion();
    std::string falling_ball_example();
    std::string oscillating_cube_example();
    std::string stable_cube_example();
    std::string stable_rolling_cube_test();
    std::string anthineas_hydrostatic_test(const std::string& type);
    std::string anthineas_exact_hydrostatic_test();
    std::string anthineas_fast_hydrostatic_test();
    std::string anthineas_propulsion();
    std::string anthineas_waves_test();
    std::string anthineas_post_processing();
    std::string outputs();
    std::string waves();
    std::string simple_waves();
    std::string cube_in_waves();
    std::string waves_for_parser_validation_only();
    std::string anthineas_froude_krylov();
    std::string falling_cube();
    std::string rolling_cube();
    std::string anthineas_damping();
    std::string controlled_forces();
    std::string wageningen();
    std::string resistance_curve();
    std::string propulsion_and_resistance();
}

#endif /* YAML_DATA_HPP_ */
