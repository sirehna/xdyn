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
    std::string bug_2655();
    std::string hydrostatic_test();
    std::string added_mass_from_file();
    std::string full_example();
    std::string full_example_with_diagonal_inertia();
    std::string full_example_with_propulsion();
    std::string falling_ball_example();
    std::string oscillating_cube_example();
    std::string new_oscillating_cube_example();
    std::string stable_cube_example();
    std::string stable_rolling_cube_test();
    std::string anthineas_hydrostatic_test(const std::string& type);
    std::string anthineas_exact_hydrostatic_test();
    std::string anthineas_fast_hydrostatic_test();
    std::string anthineas_new_hydrostatic_test();
    std::string anthineas_propulsion();
    std::string anthineas_waves_test();
    std::string anthineas_diffraction();
    std::string waves();
    std::string simple_waves();
    std::string cube_in_waves();
    std::string waves_for_parser_validation_only();
    std::string anthineas_froude_krylov();
    std::string falling_cube();
    std::string rolling_cube();
    std::string anthineas_damping();
    std::string controlled_forces();
    std::string bug_2961();
    std::string wageningen();
    std::string resistance_curve();
    std::string propulsion_and_resistance();
    std::string radiation_damping();
    std::string diffraction();
    std::string anthineas_radiation_damping();
    std::string simple_track_keeping();
    std::string maneuvering();
    std::string bug_2641();
    std::string simple_station_keeping();
    std::string rudder();
    std::string bug_in_exact_hydrostatic();
    std::string bug_2714_heading_keeping();
    std::string bug_2714_station_keeping();
    std::string bug_2732();
    std::string L_config();
    std::string GM_cube();
    std::string bug_2838();
    std::string bug_2845();
    std::string maneuvering_with_commands();
    std::string kt_kq();
    std::string linear_hydrostatics();
    std::string anthineas_linear_hydrostatics_without_waves();
    std::string anthineas_linear_hydrostatics_with_waves();
    std::string dummy_history();
    std::string complete_yaml_message_from_gui();
    std::string complete_yaml_message_for_falling_ball();
    std::string bug_2963_hs_fast();
    std::string bug_2963_hs_exact();
    std::string bug_2963_fk();
    std::string bug_2963_diff();
    std::string bug_2963_gm();
    std::string manoeuvring_with_euler_angles_and_quaternions();
}

#endif /* YAML_DATA_HPP_ */
