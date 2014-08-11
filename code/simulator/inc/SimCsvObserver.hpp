/*
 * SimCsvObserver.hpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#ifndef SIMCSVOBSERVER_HPP_
#define SIMCSVOBSERVER_HPP_

#include <ostream>
#include <string>
#include <vector>


class Sim;

/** \author cec
 *  \date Jun 19, 2014, 5:29:09 PM
 *  \brief
 *  \details
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/SimCsvObserverTest.cpp SimCsvObserverTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/SimCsvObserverTest.cpp SimCsvObserverTest expected output
 */
class SimCsvObserver
{
    public:
        SimCsvObserver(std::ostream& os);
        void observe(const Sim& sys, const double t);

    private:
        void observe_states(const Sim& sys, const double t);
        void observe_waves(const Sim& sys, const double t);
        void initialize(const Sim& sys);
        void initialize_title();
        std::ostream& os;
        bool initialized;
        std::vector<std::string> bodies;
        std::string customize(const std::string& body, const std::string anything) const;
};

#endif /* SIMCSVOBSERVER_HPP_ */
