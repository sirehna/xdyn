/*
 * SumOfWaveDirectionalSpreadings.hpp
 *
 *  Created on: Sep 3, 2014
 *      Author: cady
 */

#ifndef SUMOFWAVEDIRECTIONALSPREADINGS_HPP_
#define SUMOFWAVEDIRECTIONALSPREADINGS_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "WaveDirectionalSpreading.hpp"

/** \brief
 *  \details
 *  \addtogroup waves
 *  \ingroup waves
 *  \section ex1 Example
 *  \snippet waves/unit_tests/src/SumOfWaveDirectionalSpreadingsTest.cpp SumOfWaveDirectionalSpreadingsTest example
 *  \section ex2 Expected output
 *  \snippet waves/unit_tests/src/SumOfWaveDirectionalSpreadingsTest.cpp SumOfWaveDirectionalSpreadingsTest expected output
 */
class SumOfWaveDirectionalSpreadings : public WaveDirectionalSpreading
{
    public:
        SumOfWaveDirectionalSpreadings(const WaveDirectionalSpreading& s);
        SumOfWaveDirectionalSpreadings(const WaveDirectionalSpreading& s1, const WaveDirectionalSpreading& s2);
        SumOfWaveDirectionalSpreadings(const std::vector<WaveDirectionalSpreading>& s);

        double operator()(const double psi) const;
        WaveDirectionalSpreading* clone() const;
        std::vector<double> get_directions(const size_t n) const;

    private:
        SumOfWaveDirectionalSpreadings(); // Disabled
        std::vector<TR1(shared_ptr)<WaveDirectionalSpreading> > terms;
};

#endif /* SUMOFWAVEDIRECTIONALSPREADINGS_HPP_ */
