/*
 * SimNoObserver.hpp
 *
 *  Created on: Jun 23, 2014
 *      Author: cady
 */

#ifndef SIMNOOBSERVER_HPP_
#define SIMNOOBSERVER_HPP_

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
class SimNoObserver
{
    public:
        void observe(const Sim& sys, const double t);

    private:
};

#endif /* SIMNOOBSERVER_HPP_ */
