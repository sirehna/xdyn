/*
 * YamlWaveOutput.hpp
 *
 *  Created on: May 27, 2019
 *      Author: cady
 */

#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLWAVEOUTPUT_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLWAVEOUTPUT_HPP_

#include <string>

struct YamlWaveOutput
{
    YamlWaveOutput();
    std::string frame_of_reference;  //!< Name of the frame of reference the x & y coordinates are expressed in
    double xmin;                     //!< Minimum x value (in meters) of the points in the output mesh
    double xmax;                     //!< Maximum x value (in meters) of the points in the output mesh
    size_t nx;                       //!< Discretization of the mesh along the x axis (number of different x values)
    double ymin;                     //!< Minimum y value (in meters) of the points in the output mesh
    double ymax;                     //!< Maximum y value (in meters) of the points in the output mesh
    size_t ny;                       //!< Discretization of the mesh along the y axis (number of different x values)
};


#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLWAVEOUTPUT_HPP_ */
