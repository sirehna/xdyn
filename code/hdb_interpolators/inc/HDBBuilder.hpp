/*
 * HDBBuilder.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBBUILDER_HPP_
#define HDBBUILDER_HPP_

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
class HDBBuilder
{
    public:
        HDBBuilder(const std::string& data);
        TimestampedMatrices get_added_mass() const;
        TimestampedMatrices get_radiation_damping() const;
        RAOData get_diffraction_module() const;
        RAOData get_diffraction_phase() const;

    private:
        HDBBuilder();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* HDBBUILDER_HPP_ */
