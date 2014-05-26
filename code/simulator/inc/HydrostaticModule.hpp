/*
 * GravityModule.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICMODULE_HPP_
#define HYDROSTATICMODULE_HPP_

#include "SimulatorModuleInterface.hpp"

/** \author cec
 *  \date Apr 28, 2014, 3:19:15 PM
 *  \brief Computes the hydrostatic force acting on a given body.
 *  \details Needs the relative wave heights, g, rho.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/HydrostaticModule.cpp HydrostaticModule example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/HydrostaticModule.cpp HydrostaticModule expected output
 */
class HydrostaticModule : public SimulatorModuleInterface
{
	public:
        HydrostaticModule(DataSource* const data_source, const std::string& module_name, const std::string& body);
        HydrostaticModule(const HydrostaticModule& rhs, DataSource* const data_source);
		DataSourceModule* clone() const;
		DataSourceModule* clone(DataSource* const data_source) const;
		void update() const;
};


#endif /* HYDROSTATICMODULE_HPP_ */
