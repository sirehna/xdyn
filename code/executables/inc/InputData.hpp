/*
 * InputData.hpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#ifndef INPUTDATA_HPP_
#define INPUTDATA_HPP_

#include <string>
#include <vector>
#include <ssc/macros.hpp>
struct InputData
{
    InputData();
    std::vector<std::string> yaml_filenames;
    std::string solver;
    std::string output_filename;
    std::string wave_output;
    std::string command_file;
    double initial_timestep;
    double tstart;
    double tend;
    bool help;
    bool empty() const;
};

#endif /* INPUTDATA_HPP_ */
