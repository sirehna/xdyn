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

struct XdynCommandLineArguments
{
    XdynCommandLineArguments();
    std::vector<std::string> yaml_filenames;
    std::string solver;
    std::string output_filename;
    std::string wave_output;
    double initial_timestep;
    double tstart;
    double tend;
    bool catch_exceptions;
    bool empty() const;
};

#endif /* INPUTDATA_HPP_ */
