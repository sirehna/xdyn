/*
 * SimObserver.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef SIMOBSERVER_HPP_
#define SIMOBSERVER_HPP_

#include <map>
#include <string>
#include <vector>

class Sim;

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
        SimObserver(const Sim& bodies);
        void observe(const Sim& sys, const double t);
        std::vector<std::map<std::string,double> > get() const;

    private:
        std::vector<std::string> bodies;
        std::string customize(const std::string& body, const std::string anything) const;
        std::vector<std::map<std::string,double> > res;
};

#endif /* SIMOBSERVER_HPP_ */
