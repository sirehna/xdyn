/*
 * StateDerivativesModule.hpp
 *
 *  Created on: May 29, 2014
 *      Author: cady
 */

#ifndef STATEDERIVATIVESMODULE_HPP_
#define STATEDERIVATIVESMODULE_HPP_

#include <vector>

#include "SimulatorModuleInterface.hpp"

/** \author cec
 *  \date May 29, 2014, 8:14:19 PM
 *  \brief Computes the sum of the external forces acting on a body & the state derivatives
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/StateDerivativesModuleTest.cpp StateDerivativesModuleTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/StateDerivativesModuleTest.cpp StateDerivativesModuleTest expected output
 */
class StateDerivativesModule : public SimulatorModuleInterface
{
    public:
        StateDerivativesModule(DataSource* const data_source, const std::string& body, const std::vector<std::string>& forces);
        StateDerivativesModule(const StateDerivativesModule& rhs, DataSource* const data_source);
        DataSourceModule* clone() const;
        DataSourceModule* clone(DataSource* const data_source) const;
        void update() const;

    private:
        std::vector<std::string> forces;
};

#endif /* STATEDERIVATIVESMODULE_HPP_ */
