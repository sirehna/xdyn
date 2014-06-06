#ifndef SIMULATORAPI_HPP_
#define SIMULATORAPI_HPP_

#include <map>
#include <string>

#include "GeometricTypes3d.hpp"
#include "DataSource.hpp"

/**  \author cec
  *  \date Jun 6, 2014, 10:36:46 AM
  *  \brief Generates a DataSource from YAML & solver data
  *  \details This function is used directly in integration tests & the 'sim'
  *  executable calls it too.
  *  \returns A DataSource, ready for simulation.
  *  \snippet simulator/unit_tests/src/simulator_apiTest.cpp simulator_apiTest API_example
  */
DataSource make_ds(const std::string& data, //!< YAML data
                   const double dt, //!< Initial time step (for adaptive steppers) or value of the time step (for constant-step steppers) (in seconds)
                   const std::string& solver_type //!< Name of the solver: euler,rk4,rkck for Euler, Runge-Kutta 4 & Runge-Kutta-Cash-Karp respectively
                   );

/**  \author cec
  *  \date Jun 6, 2014, 10:36:46 AM
  *  \brief Generates a DataSource from YAML, mesh & solver data (for tests)
  *  \details This function is used directly in integration tests. It is *not*
  *  used by the 'sim'
  *  \returns A DataSource, ready for simulation.
  *  \snippet simulator/unit_tests/src/simulator_apiTest.cpp simulator_apiTest API_example
  */
DataSource make_ds(const std::string& data, //!< YAML data
                   const std::map<std::string, VectorOfVectorOfPoints>& input_meshes, // Mesh data for each body described in the YAML data
                   const double dt, //!< Initial time step (for adaptive steppers) or value of the time step (for constant-step steppers) (in seconds)
                   const std::string& solver_type //!< Name of the solver: euler,rk4,rkck for Euler, Runge-Kutta 4 & Runge-Kutta-Cash-Karp respectively
                   );

#endif
