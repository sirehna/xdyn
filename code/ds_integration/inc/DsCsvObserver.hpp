/*
 * DsCsvObserver.hpp
 *
 *  Created on: 26 mars 2014
 *      Author: maroff
 */

#ifndef DSCSVOBSERVER_HPP_
#define DSCSVOBSERVER_HPP_

#include <ostream>
class DSSystem;

/** \author cec
 *  \brief Outputs the simulation results (in CSV form) to a stream.
 *  \details The difference with the DefaultObserver is that the variable names
 *  are given by the DataSource, instead of just being x0, x1,...
 *  \ingroup ds_integration
 *  \section ex1 Example
 *  \snippet ds_integration/unit_tests/src/DsCsvObserverTest.cpp DsCsvObserverTest example
 *  \section ex2 Expected output
 *  \snippet ds_integration/unit_tests/src/DsCsvObserverTest.cpp DsCsvObserverTest expected output
 */
class DsCsvObserver
{
    public:
        DsCsvObserver(std::ostream& os);
        void observe(const DSSystem& sys, const double t);

    private:
        std::ostream& os;
        bool initialized;
};

#endif /* DSCSVOBSERVER_HPP_ */
