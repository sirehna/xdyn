/*
 * DiffractionInterpolator.cpp
 *
 *  Created on: Dec 17, 2014
 *      Author: cady
 */

#include "DiffractionInterpolator.hpp"
#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "HDBParser.hpp"

DiffractionInterpolator::DiffractionInterpolator(const HDBParser& data, //!< Data read from the HDB file
                                             const std::vector<double>& omega, //!< Angular frequencies in the wave spectrum (points at which to interpolate the HDB data)
                                             const std::vector<double>& psi, //!< Wave directions (points at which to interpolate the HDB data)
                                             const bool mirror_ //!< Should the RAO for psi between 180° and 360° be calculated by mirroring the RAO between 0° and 180°?
        ) : module(), phase(), mirror(mirror_), omegas(omega), psis(psi)
{
    const auto M_module = data.get_diffraction_module_tables();
    const auto M_phase = data.get_diffraction_phase_tables();

    // For each axis (X,Y,Z,phi,theta,psi)
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        // module.at(i) and phase.at(i) are vectors of vectors: each element in the vector of vectors corresponds to a frequency omega
        // For each omega, we have a vector containing the RAO values for each incidence
        module.at(i) = Interpolator(data.get_diffraction_module_omegas(),data.get_diffraction_module_psis(),M_module.at(i));
        phase.at(i) = Interpolator(data.get_diffraction_phase_omegas(),data.get_diffraction_phase_psis(),M_phase.at(i));
    }
}

std::vector<std::vector<double> > DiffractionInterpolator::get_array_cartesian(Interpolator& i) const
{
    std::vector<std::vector<double> > ret;
    for (auto omega:omegas)
    {
        std::vector<double> v;
        for (auto psi:psis)
        {
            if (mirror and (psi>PI)) v.push_back(i.f(omega,2*PI-psi));
            else                     v.push_back(i.f(omega,psi));
        }
        ret.push_back(v);
    }
    return ret;
}

std::vector<std::vector<double> > DiffractionInterpolator::get_modules_cartesian(const size_t k //!< Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
                                                     )
{
    return get_array_cartesian(module.at(k));
}

std::vector<std::vector<double> > DiffractionInterpolator::get_phases_cartesian(const size_t k //!< Axis index (0 for Fx, 1 for Fy, 2 for Fz, 3 for Mx, 4 for My and 5 for Mz)
                                         )
{
    return get_array_cartesian(phase.at(k));
}

double DiffractionInterpolator::interpolate_module(const size_t axis, const double omega, const double beta)
{
    if (mirror and (beta>PI)) return module.at(axis).f(omega,2*PI-beta);
                              return module.at(axis).f(omega,beta);
}

double DiffractionInterpolator::interpolate_phase(const size_t axis, const double omega, const double beta)
{
    if (mirror and (beta>PI)) return phase.at(axis).f(omega,2*PI-beta);
                              return phase.at(axis).f(omega,beta);
}
