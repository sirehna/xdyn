/*
 * HDBParserForTests.hpp
 *
 *  Created on: Dec 2, 2014
 *      Author: cady
 */

#ifndef HDBPARSERFORTESTS_HPP_
#define HDBPARSERFORTESTS_HPP_

#include "HDBParser.hpp"

class HDBParserForTests : public HDBParser
{
    public:
        HDBParserForTests(const std::vector<double>& omega, const std::vector<double>& Br, const bool only_diagonal_terms_=true);
        std::vector<double> get_radiation_damping_angular_frequencies() const;
        std::vector<double> get_radiation_damping_coeff(const size_t i, const size_t j) const;

    private:
        HDBParserForTests();
        std::vector<double> omega;
        std::vector<double> Br;
        bool only_diagonal_terms;
};

#endif /* HDBPARSERFORTESTS_HPP_ */
