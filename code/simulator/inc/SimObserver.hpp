/*
 * SimObserver.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef SIMOBSERVER_HPP_
#define SIMOBSERVER_HPP_

#include "Res.hpp"

#include <map>
#include <string>
#include <vector>

class Sim;
class YamlSimulatorInput;

/** \author cec
 *  \date Jun 17, 2014, 8:23:16 AM
 *  \brief
 *  \details
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/SimObserverTest.cpp SimObserverTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/SimObserverTest.cpp SimObserverTest expected output
 */
class SimObserver
{
    public:
        SimObserver();
        void observe(const Sim& sys, const double t);
        std::vector<Res> get() const;

    private:
        std::vector<Res> res;
};

#endif /* SIMOBSERVER_HPP_ */
