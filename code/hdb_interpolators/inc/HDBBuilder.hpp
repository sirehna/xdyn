/*
 * HDBBuilder.hpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#ifndef HDBBUILDER_HPP_
#define HDBBUILDER_HPP_

#include "hdb_parser_internal_data_structures.hpp"
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

    private:
        HDBBuilder();

        void fill(TimestampedMatrices& ret, const size_t i, const hdb::ListOfValues& M) const;
        hdb::AST tree;
};

#endif /* HDBBUILDER_HPP_ */
