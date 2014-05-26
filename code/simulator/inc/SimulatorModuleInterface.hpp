/*
 * SimulatorModuleInterface.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef SIMULATORMODULEINTERFACE_HPP_
#define SIMULATORMODULEINTERFACE_HPP_

#include "DataSourceModule.hpp"

/** \author cec
 *  \date May 22, 2014, 4:16:15 PM
 *  \brief Provides a unified interface for all modules operating on a single body.
 *  \ingroup simulator
 */
class SimulatorModuleInterface : public DataSourceModule
{
	public:
        SimulatorModuleInterface(DataSource* const data_source, const std::string& module_name, const std::string& body);
        SimulatorModuleInterface(const SimulatorModuleInterface& rhs, DataSource* const data_source);
		virtual DataSourceModule* clone() const = 0;
		virtual DataSourceModule* clone(DataSource* const data_source) const = 0;
		virtual void update() const = 0;

	protected:
		std::string customize(const std::string& name) const;
		std::string body;
};


#endif /* SIMULATORMODULEINTERFACE_HPP_ */
