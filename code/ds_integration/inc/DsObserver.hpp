/*
 * DsObserver.hpp
 *
 *  Created on: 31 mars 2014
 *      Author: cec
 */

#ifndef DSOBSERVER_HPP_
#define DSOBSERVER_HPP_

#include <ostream>

class DSSystem;

/** \author cec
 *  \brief Interface to a DsSystem observer for ODE integration (as defined in the simulator module)
 *  \ingroup ds_integration
 *  \section ex1 Example
 *  \snippet ds_integration/unit_tests/src/DsObserverTest.cpp DsObserverTest example
 *  \section ex2 Expected output
 *  \snippet ds_integration/unit_tests/src/DsObserverTest.cpp DsObserverTest expected output
 */
class DsObserver
{
    public:
        DsObserver();
        virtual ~DsObserver();
        virtual void observe(const DSSystem& sys, const double t) = 0;
};

#endif /* DSOBSERVER_HPP_ */
