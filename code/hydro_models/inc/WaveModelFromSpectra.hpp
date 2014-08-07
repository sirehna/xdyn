/*
 * WaveModelFromSpectra.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef WAVEMODELFROMSPECTRA_HPP_
#define WAVEMODELFROMSPECTRA_HPP_

#include "WaveModelInterface.hpp"

class WaveModel;

/** \brief Multiple (directional spreading+spectrum) pairs
 *  \details This is just a very thin layer around the WaveModel class.
 *  \addtogroup hydro_models
 *  \ingroup hydro_models
 *  \section ex1 Example
 *  \snippet hydro_models/unit_tests/src/DefaultWaveModelTest.cpp DefaultWaveModelTest example
 *  \section ex2 Expected output
 *  \snippet hydro_models/unit_tests/src/DefaultWaveModelTest.cpp DefaultWaveModelTest expected output
 */
class WaveModelFromSpectra : public WaveModelInterface
{
    public:
        WaveModelFromSpectra(const TR1(shared_ptr)<WaveModel>& model);

    private:
        WaveModelFromSpectra(); // Disabled

         /*  \snippet hydro_models/unit_tests/src/DefaultWaveModelTest.cpp DefaultWaveModelTest wave_height_example
          */
        double wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double z, //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double t  //!< Current instant (in seconds)
                           ) const;

        TR1(shared_ptr)<WaveModel> model;
};
#endif /* WAVEMODELFROMSPECTRA_HPP_ */
