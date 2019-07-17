/*
 * Res.hpp
 *
 *  Created on: Jul 1, 2014
 *      Author: cady
 */

#ifndef RES_HPP_
#define RES_HPP_

#include <map>
#include <string>
#include "StateMacros.hpp"

/** \author cec
 *  \date Jul 1, 2014, 9:28:53 AM
 *  \brief Stores simulation results (states & instant)
 *  \details This structure will be outputted at each simulation step.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/ResTest.cpp ResTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/ResTest.cpp ResTest expected output
 */
struct Res
{
    Res();
    Res(const double t_, const StateType& x_);
    double t; //!< Instant at which the states correspond
    StateType x; //!< Values of the states of the system
    std::map<std::string, double> extra_observations; //!< extra outputs from force models. Corresponds to 'extra_observations' in code/grpc/force.proto#L99
};

#endif /* RES_HPP_ */
