/*
 * env_for_tests.hpp
 *
 *  Created on: Feb 3, 2015
 *      Author: cady
 */


#ifndef ENV_FOR_TESTS_HPP_
#define ENV_FOR_TESTS_HPP_

#include "EnvironmentAndFrames.hpp"

#define BODY "body 1"

class WaveModel;

EnvironmentAndFrames get_environment_and_frames(const TR1(shared_ptr)<WaveModel>& wave_model);

#endif  /* ENV_FOR_TESTS_HPP_ */
