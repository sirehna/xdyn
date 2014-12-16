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
        WaveModel(const DiscreteDirectionalWaveSpectrum& spectrum);
        virtual ~WaveModel();

        /**  \author cec
          *  \date Aug 1, 2014, 3:24:45 PM
          *  \brief Surface elevation
          *  \returns Elevation of a point at a given instant, in meters.
          *  \snippet wave_models/unit_tests/src/WaveModelTest.cpp WaveModelTest method_example
          */
        virtual double elevation(const double x,//!< x-position in the NED frame (in meters)
                         const double y,//!< y-position in the NED frame (in meters)
                         const double t //!< Current time instant (in seconds)
                         ) const = 0;

        /**  \brief Pressure induced by waves
          *  \returns Pressure (in Pa) induced by the waves, at a given point in the fluid
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.20, page 125
          *  \snippet wave_models/unit_tests/src/AiryTest.cpp AiryTest elevation_example
          */
        virtual double dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                        const double g,   //!< gravity (in m/s^2)
                                        const double x,   //!< x-position in the NED frame (in meters)
                                        const double y,   //!< y-position in the NED frame (in meters)
                                        const double z,   //!< z-position in the NED frame (in meters)
                                        const double eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
                                        const double t    //!< Current time instant (in seconds)
                                        ) const = 0;

        /**  \returns List of angular frequencies for which the spectra will be calculated.
          *  \details Needed by the RAOs (RadiationForceModel)
          */
        std::vector<double> get_omegas() const;

        /**  \returns List of incidences for which the spectra will be calculated.
          *  \details Needed by the RAOs (RadiationForceModel)
          */
        std::vector<double> get_psis() const;

    private:
        WaveModel(); // Disabled

    protected:
        DiscreteDirectionalWaveSpectrum spectrum;
};

#endif /* WAVEMODEL_HPP_ */
