/*
 * HDBBuilder.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBBUILDER_HPP_
#define HDBBUILDER_HPP_

#include <Eigen/Dense>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "TimestampedMatrix.hpp"

/** \brief
 *  \details
 *  \addtogroup hdb_interpolators
 *  \ingroup hdb_interpolators
 *  \section ex1 Example
 *  \snippet hdb_interpolators/unit_tests/src/HDBBuilderTest.cpp HDBBuilderTest example
 *  \section ex2 Expected output
 *  \snippet hdb_interpolators/unit_tests/src/HDBBuilderTest.cpp HDBBuilderTest expected output
 */
class HDBParser
{
    public:
        HDBParser(const std::string& data);
        TimestampedMatrices get_added_mass_array() const;
        TimestampedMatrices get_radiation_damping_array() const;
        RAOData get_diffraction_module() const;
        RAOData get_diffraction_phase() const;
        Eigen::Matrix<double,6,6> get_added_mass() const;
        Eigen::Matrix<double,6,6> get_added_mass(const double Tp //!< Period at which to interpolate the added mass
                                                ) const; // const doesn't really mean anything here as the members are hidden inside a pimpl
        std::vector<double> get_radiation_damping_angular_frequencies() const;
        std::vector<double> get_radiation_damping_coeff(const size_t i, const size_t j) const;

    private:
        HDBParser();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* HDBBUILDER_HPP_ */
