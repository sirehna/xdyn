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

struct InputData
{
    InputData();
    std::vector<std::string> yaml_filenames;
    std::string output_csv;
    std::string solver;
    double initial_timestep;
    double tstart;
    double tend;
    bool help;
};


#endif /* INPUTDATA_HPP_ */
