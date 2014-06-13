/*
 * DsSystemMacros.hpp
 *
 *  Created on: 25 mars 2014
 *      Author: cec
 */

#ifndef DSSYSTEMMACROS_HPP_
#define DSSYSTEMMACROS_HPP_

#include <set>
#include "DataSource.hpp"

#define FORCE_MODULE(ns, x, code) namespace ns {namespace x{static const std::string signal_name = #ns "_" #x;MODULE(ForceModule, code)}}
#define APPEND_FORCE(x, ds) ds.add<x::ForceModule>(#x);\
                                try\
                                {\
                                        ds.get<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name);\
                                }\
                                catch (const DataSourceException& exc)\
                                {\
                                    ds.set<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name,simulator_base::list_of_forces::_type());\
                                }\
                                    {\
                                        auto __v = ds.get<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name);\
                                        __v.insert(x::signal_name);\
                                        ds.set<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name,__v);\
                                    }

DEFINE(simulator_base, list_of_forces, std::set<std::string>)
DEFINE(simulator_base, initial_time_step, double)

#define DEFINE_DERIVATIVE(x, dx_dt, ds) ds.define_derivative(x::_name, dx_dt::_name);

namespace solver
{
    typedef enum {RK4, EULER, RKCK54} SolverType;
}

DEFINE(simulator_base, stepper, solver::SolverType)

#endif /* DSSYSTEMMACROS_HPP_ */
