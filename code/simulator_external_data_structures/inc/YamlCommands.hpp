/*
 * YamlCommands.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef YAMLCOMMANDS_HPP_
#define YAMLCOMMANDS_HPP_

#include <map>
#include <string>
#include <vector>

/** \brief Stores the commands read from the command parameter file (for controlled forces)
 */
struct YamlCommands
{
    YamlCommands();
    std::string name;                                    //!< Name of the controlled force
    std::vector<double> t;                               //!< Time instants at which the commands change
    std::map<std::string,std::vector<double> > commands; //!< List of command values at each instant
};


#endif /* YAMLCOMMANDS_HPP_ */
