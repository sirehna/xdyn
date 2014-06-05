/*
 * DsMapObserver.hpp
 *
 *  Created on: Jun 5, 2014
 *      Author: cady
 */

#ifndef DSMAPOBSERVER_HPP_
#define DSMAPOBSERVER_HPP_

#include <map>
#include <vector>

#include "DsObserver.hpp"

/** \author cec
 *  \date Jun 5, 2014, 4:09:41 PM
 *  \brief Outputs the simulation results to a std::map
 *  \details Each element of the vector corresponds to a time step.
 *  The map contains the values of t plus all the states.
 *  \ingroup ds_integration
 *  \section ex1 Example
 *  \snippet ds_integration/unit_tests/src/DsMapObserverTest.cpp DsMapObserverTest example
 *  \section ex2 Expected output
 *  \snippet ds_integration/unit_tests/src/DsMapObserverTest.cpp DsMapObserverTest expected output
 */
class DsMapObserver : public DsObserver
{
    public:
        DsMapObserver();
        void observe(const DSSystem& sys, const double t);
        std::vector<std::map<std::string,double> > get() const;

    private:
        std::vector<std::map<std::string,double> > v;
};

#endif /* DSMAPOBSERVER_HPP_ */
