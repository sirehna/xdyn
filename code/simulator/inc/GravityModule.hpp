/*
 * GravityModule.hpp
 *
 *  Created on: Apr 28, 2014
 *      Author: cady
 */

#ifndef GRAVITYMODULE_HPP_
#define GRAVITYMODULE_HPP_

#include "DataSourceModule.hpp"

/** \author cec
 *  \date Apr 28, 2014, 3:19:15 PM
 *  \brief Computes the gravity force of a given body.
 *  \details Needs the mass & the constant of gravity.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/GravityModuleTest.cpp GravityModuleTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/GravityModuleTest.cpp GravityModuleTest expected output
 */
class GravityModule : public DataSourceModule
{
	public:
		GravityModule(DataSource* const data_source, const std::string& module_name, const std::string& body);
		GravityModule(const GravityModule& rhs, DataSource* const data_source);
		DataSourceModule* clone() const;
		DataSourceModule* clone(DataSource* const data_source) const;
		void update() const;

	private:
		std::string body;
};


#endif /* GRAVITYMODULE_HPP_ */
