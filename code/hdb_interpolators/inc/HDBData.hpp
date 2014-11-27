/*
 * HDBData.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBDATA_HPP_
#define HDBDATA_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <Eigen/Dense>

#include "TimestampedMatrix.hpp"

class HDBBuilder;

/** \brief
 *  \details
 *  \addtogroup hdb_interpolators
 *  \ingroup hdb_interpolators
 *  \section ex1 Example
 *  \snippet hdb_interpolators/unit_tests/src/HDBDataTest.cpp HDBDataTest example
 *  \section ex2 Expected output
 *  \snippet hdb_interpolators/unit_tests/src/HDBDataTest.cpp HDBDataTest expected output
 */
class HDBData
{
    public:
        HDBData(const HDBBuilder& builder);
        Eigen::Matrix<double,6,6> get_added_mass() const;
        Eigen::Matrix<double,6,6> get_added_mass(const double Tp //!< Period at which to interpolate the added mass
                                                ) const; // const doesn't really mean anything here as the members are hidden inside a pimpl
    private:
        HDBData();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* HDBDATA_HPP_ */
