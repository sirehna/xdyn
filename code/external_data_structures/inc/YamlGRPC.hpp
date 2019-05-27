/*
 * YamlGRPC.hpp
 *
 *  Created on: May 27, 2019
 *      Author: cady
 */

#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLGRPC_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLGRPC_HPP_

#include <string>

#include "YamlWaveOutput.hpp"

struct YamlGRPC
{
    YamlGRPC();
    std::string url;
    std::string rest_of_the_yaml;
    YamlWaveOutput output; //!< Defines what wave data is outputted during the simulation & how it is generated
};


#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLGRPC_HPP_ */
