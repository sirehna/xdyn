/*
 * DefaultWaveModel.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef DEFAULTWAVEMODEL_HPP_
#define DEFAULTWAVEMODEL_HPP_

#include "WaveModelInterface.hpp"

/** \author cec
 *  \date 24 avr. 2014, 10:15:24
 *  \brief Flat sea surface.
 *  \details The relative height is zwave-z, so a negative
 *           value implies the point is above the water and a
 *           positive value implies it's underneath. This convention
 *           was chosen because it should simplify the computation of the
 *           hydrostatic forces (no sign change).
 *  \ingroup hydro_models
 *  \section ex1 Example
 *  \snippet hydro_models/unit_tests/src/DefaultWaveModelTest.cpp DefaultWaveModelTest example
 *  \section ex2 Expected output
 *  \snippet hydro_models/unit_tests/src/DefaultWaveModelTest.cpp DefaultWaveModelTest expected output
 */
class DefaultWaveModel : public WaveModelInterface
{
    public:
        DefaultWaveModel(const double wave_height);
        double get_relative_wave_height(const Point& P) const;

    private:
        DefaultWaveModel(); // Disabled
        double wave_height;
};

#endif /* DEFAULTWAVEMODEL_HPP_ */
