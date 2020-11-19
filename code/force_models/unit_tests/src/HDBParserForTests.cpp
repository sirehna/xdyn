/*
 * HDBParserForTests.cpp
 *
 *  Created on: Dec 2, 2014
 *      Author: cady
 */

#include "HDBParserForTests.hpp"

HDBParserForTests::HDBParserForTests(const std::vector<double>& omega_, const std::vector<double>& Br_, const bool only_diagonal_terms_) : omega(omega_), Br(Br_), only_diagonal_terms(only_diagonal_terms_)
{
}

std::vector<double> HDBParserForTests::get_radiation_damping_angular_frequencies() const
{
    return omega;
}

std::vector<double> HDBParserForTests::get_radiation_damping_coeff(const size_t i, const size_t j) const
{
    if (only_diagonal_terms)
    {
        if (i == j)
        {
            return Br;
        }
    }
    else
    {
        std::vector<double> ret;
        ret.reserve(Br.size());
        for (const auto br:Br)
        {
            ret.push_back((10*(i+1)+(j+1))*br);
        }
        return ret;
    }
    return std::vector<double>(Br.size(),0);
}
