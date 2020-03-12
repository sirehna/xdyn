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
    std::string full_example_with_propulsion_and_old_key_name();
    std::string falling_ball_example();
    std::string oscillating_cube_example();
    std::string new_oscillating_cube_example();
    std::string stable_cube_example();
    std::string stable_rolling_cube_test();
    std::string test_ship_hydrostatic_test(const std::string& type);
    std::string test_ship_exact_hydrostatic_test();
    std::string test_ship_fast_hydrostatic_test();
    std::string test_ship_new_hydrostatic_test();
    std::string test_ship_propulsion();
    std::string test_ship_waves_test();
    std::string test_ship_diffraction();
    std::string waves();
    std::string simple_waves();
    std::string cube_in_waves();
    std::string waves_for_parser_validation_only();
    std::string test_ship_froude_krylov();
    std::string falling_cube();
    std::string rolling_cube();
    std::string test_ship_damping();
    std::string controlled_forces();
    std::string bug_2961();
    std::string wageningen();
    std::string resistance_curve();
    std::string propulsion_and_resistance();
    std::string radiation_damping();
    std::string diffraction();
    std::string test_ship_radiation_damping();
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
    std::string test_ship_linear_hydrostatics_without_waves();
    std::string test_ship_linear_hydrostatics_with_waves();
    std::string dummy_history();
    std::string complete_yaml_message_from_gui();
    std::string complete_yaml_message_for_falling_ball();
    std::string bug_2963_hs_fast();
    std::string bug_2963_hs_exact();
    std::string bug_2963_fk();
    std::string bug_2963_diff();
    std::string bug_2963_gm();
    std::string manoeuvring_with_euler_angles_and_quaternions();
    std::string bug_3004();
    std::string bug_3003();
    std::string bug_2984();
    std::string bug_3217();
    std::string bug_3227();
    std::string yml_bug_3230();
    std::string bug_3235();
    std::string bug_3207_yml();
    std::string bug_3230_yml();
    std::string hos();
    std::string hos_for_parser_validation_only();
    std::string bug_3241();
    std::string constant_force();
    std::string issue_20();
    std::string simserver_test_with_commands_and_delay();
    std::string simserver_message_without_Dt();
    std::string man_with_delay();
    std::string invalid_json_for_cs();
    std::string tutorial_09_gRPC_wave_model();
    std::string tutorial_10_gRPC_force_model();
    std::string tutorial_10_gRPC_force_model_commands();
    std::string gRPC_force_model();
}

#endif /* YAML_DATA_HPP_ */
