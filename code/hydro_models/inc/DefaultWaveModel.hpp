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
        DefaultWaveModel(const double wave_height, const std::tr1::shared_ptr<Kinematics>& k);

    private:
        DefaultWaveModel(); // Disabled

        /**  \author cec
          *  \date 24 avr. 2014, 10:57:09
          *  \brief Flat sea surface model.
          *  \returns zwave - z;
          *  \snippet hydro_models/unit_tests/src/DefaultWaveModelTest.cpp DefaultWaveModelTest wave_height_example
          */
        double wave_height(double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           double z  //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           ) const;

        double zwave;
};

#endif /* DEFAULTWAVEMODEL_HPP_ */
