/*
 * WaveModule.hpp
 *
 *  Created on: May 22, 2014
 *      Author: cady
 */

#ifndef WAVEMODULE_HPP_
#define WAVEMODULE_HPP_

#include "SimulatorModuleInterface.hpp"

/** \author cec
 *  \date Apr 28, 2014, 3:19:15 PM
 *  \brief Computes the relative wave heights of all points on a mesh.
 *  \details Needs the mesh & wave model.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/WaveModuleTest.cpp WaveModuleTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/WaveModuleTest.cpp WaveModuleTest expected output
 */
class WaveModule : public SimulatorModuleInterface
{
    public:
        WaveModule(DataSource* const data_source, const std::string& module_name, const std::string& body);
        WaveModule(const WaveModule& rhs, DataSource* const data_source);
        DataSourceModule* clone() const;
        DataSourceModule* clone(DataSource* const data_source) const;
        void update() const;
};

#endif /* WAVEMODULE_HPP_ */
