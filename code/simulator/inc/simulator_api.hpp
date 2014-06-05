#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "GeometricTypes3d.hpp"
#include "DataSource.hpp"

DataSource make_ds(const std::string& data);

DataSource make_ds(const std::string& data,
                   const std::map<std::string, VectorOfVectorOfPoints>& input_meshes);

#endif
