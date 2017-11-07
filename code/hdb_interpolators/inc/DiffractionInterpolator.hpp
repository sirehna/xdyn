/*
 * DiffractionInterpolator.hpp
 *
 *  Created on: Dec 17, 2014
 *      Author: cady
 */

#ifndef DIFFRACTIONINTERPOLATOR_HPP_
#define DIFFRACTIONINTERPOLATOR_HPP_

#include <array>
#include <cstdlib> // For size_t
#include <vector>

#include <ssc/interpolation.hpp>

class HDBParser;

typedef ssc::interpolation::TwoDimensionalInterpolationVariableStep Interpolator;

/** \brief Interpolates the RAO read from the HDB file to the (omega,psi) grid of the wave model
 *  \details Calculates the inputs of WaveModel::evaluate_rao
 *  \addtogroup hdb_interpolators
 *  \ingroup hdb_interpolators
 *  \section ex1 Example
 *  \snippet hdb_interpolators/unit_tests/src/RadiationInterpolatorTest.cpp RadiationInterpolatorTest example
 *  \section ex2 Expected output
 *  \snippet hdb_interpolators/unit_tests/src/RadiationInterpolatorTest.cpp RadiationInterpolatorTest expected output
 */
class DiffractionInterpolator
{
    public:
        DiffractionInterpolator(const HDBParser& data, //<! Data read from the HDB file
                                const std::vector<double>& omega, //<! Angular frequencies in the wave spectrum (points at which to interpolate the HDB data)
                                const std::vector<double>& psi, //<! Wave directions (points at which to interpolate the HDB data)
                                const bool mirror //<! Should the RAO for psi between 180° and 360° be calculated by mirroring the RAO between 0° and 180°?
        );

        /**  \brief Interpolates the RAO's module for the values of omega & psi in the wave spectrum
          *  \returns RAO_module[i][j], where i is frequency index & j direction index
          *  \snippet hdb_interpolators/unit_tests/src/RadiationInterpolatorTest.cpp RadiationInterpolatorTest method_example
          */
        std::vector<std::vector<double> > get_modules_cartesian(const size_t k //<! Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
                                                               );

        /**  \brief Interpolates the RAO's phase for the values of omega & psi in the wave spectrum
          *  \returns RAO_phase[i][j], where i is frequency index & j direction index
          *  \snippet hdb_interpolators/unit_tests/src/RadiationInterpolatorTest.cpp RadiationInterpolatorTest method_example
          */
        std::vector<std::vector<double> > get_phases_cartesian(const size_t k //<! Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
                                                              );

        /**  \brief Interpolates the RAO's module for a given axis, a wave period (*not* an encounter period)
         *    and an incidence (between 0 and π for waves propagating towards port side, π and 2π for waves propagating towards starboard
         *    0 for waves propagating from aft to fore (heading waves), pi for waves propagating from fore to aft (facing waves))
          *  \returns module of the rao for the axis, the frequency and the incidence
          */
        double interpolate_module(const size_t axis, const double Tp, double beta);

        /**  \brief Interpolates the RAO's phase for a given axis, a wave period (*not* an encounter period)
         *    and an incidence (between 0 and π for waves propagating towards port side, π and 2π for waves propagating towards starboard
         *    0 for waves propagating from aft to fore (heading waves), pi for waves propagating from fore to aft (facing waves))
          *  \returns module of the rao for the axis, the frequency and the incidence
          */
        double interpolate_phase(const size_t axis, const double Tp, double beta);

    private:
        DiffractionInterpolator();
        std::vector<std::vector<double> > get_array_cartesian(Interpolator& i) const;
    public:
        std::array<Interpolator,6> module; //!< 2D spline interpolation function (omega,psi) for RAO's module
        std::array<Interpolator,6> phase; //!< 2D spline interpolation function (omega,psi) for RAO's phase
        bool mirror;
        std::vector<double> omegas;
        std::vector<double> psis;
};

#endif /* DIFFRACTIONINTERPOLATOR_HPP_ */
