#ifndef SIMHDF5OBSERVERBUILDER_HPP_
#define SIMHDF5OBSERVERBUILDER_HPP_

#include <vector>
#include "H5Cpp.h"
#include "h5_interface.hpp"
#include "Sim.hpp"

struct H5Res
{
    std::vector<double> v; //!< Values
    H5Res() : v(){}
    H5Res(const std::vector<double>& v_):v(v_){}
    H5Res(const double t_, const std::vector<double>& v_):v(){v.push_back(t_);v.insert(v.end(),v_.begin(),v_.end());}
};

template <> void H5_Serialize<H5Res>::write(H5Res const * const data);

H5::CompType H5_CreateIdStates(const VectorOfStringModelForEachBody& v);
H5::CompType H5_CreateIdEfforts(const VectorOfStringModelForEachBody& v);
H5::CompType H5_CreateIdQuaternion();
H5::CompType H5_CreateIdEulerAngle();
H5::CompType H5_CreateIdWrenchType();

#endif
