#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "GeometricTypes3d.hpp"
#include "Sim.hpp"

Sim get_system(const std::string& yaml);
Sim get_system(const std::string& yaml, const std::string& mesh);
Sim get_system(const std::string& yaml, const std::map<std::string, VectorOfVectorOfPoints>& meshes);

#endif
