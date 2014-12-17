/*
 * YamlDiffraction.hpp
 *
 */

#ifndef YAMLDIFFRACTION_HPP_
#define YAMLDIFFRACTION_HPP_

#include <string>

#include "YamlCoordinates.hpp"
/*
 *     ss << "model: diffraction\n"
       << "hdb: anthineas.hdb\n"
       << "calculation point in body frame:\n"
       << "    x: {value: 0.696, unit: m}\n"
       << "    y: {value: 0, unit: m}\n"
       << "    z: {value: 1.418, unit: m}\n"
       << "mirror for 180 to 360: true\n";
 */
struct YamlDiffraction
{
    YamlDiffraction();
    std::string     hdb_filename;
    YamlCoordinates calculation_point;
    bool            mirror;
};

#endif /* YAMLDIFFRACTION_HPP_ */
