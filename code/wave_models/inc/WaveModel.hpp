/*
 * WaveModel.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#ifndef WAVEMODEL_HPP_
#define WAVEMODEL_HPP_

#include "DiscreteDirectionalWaveSpectrum.hpp"


/** \author cec
 *  \date Aug 1, 2014, 3:15:04 PM
 *  \brief Interface to wave models.
 *  \details Should at least provide elevation.
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/WaveModelTest.cpp WaveModelTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/WaveModelTest.cpp WaveModelTest expected output
 */
class WaveModel
{
    public:
        WaveModel(const std::vector<DiscreteDirectionalWaveSpectrum>& spectra);
        virtual ~WaveModel();

        /**  \author cec
          *  \date Aug 1, 2014, 3:24:45 PM
          *  \brief Surface elevation
          *  \returns Elevation of a point at a given instant, in meters.
          *  \snippet wave_models/unit_tests/src/WaveModelTest.cpp WaveModelTest method_example
          */
        double elevation(const double x,//!< x-position in the NED frame (in meters)
                         const double y,//!< y-position in the NED frame (in meters)
                         const double t //!< Current time instant (in seconds)
                         ) const;

    private:
        WaveModel(); // Disabled

        /**  \author cec
          *  \date Aug 1, 2014, 3:46:02 PM
          *  \brief Will be called by the public elevation method for each spectrum.
          *  \returns Elevation due to one directional spectrum
          *  \snippet wave_models/unit_tests/src/WaveModelTest.cpp WaveModelTest method_example
          */
        virtual double elevation_single_spectrum(const DiscreteDirectionalWaveSpectrum::Output& spectrum, //!< Discrete spectrum under consideration
                                                 const double x,                                  //!< x-position in the NED frame (in meters)
                                                 const double y,                                  //!< y-position in the NED frame (in meters)
                                                 const double t                                   //!< Current time instant (in seconds)
                                                ) const = 0;

        std::vector<DiscreteDirectionalWaveSpectrum::Output> spectra;
};

#endif /* WAVEMODEL_HPP_ */
