/*
 * discretize.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include <cmath> //std::sqrt
#include <list>
#include <utility> //std::pair

#include "discretize.hpp"
#include "WaveDirectionalSpreading.hpp"
#include "WaveSpectralDensity.hpp"

DiscreteDirectionalWaveSpectrum common(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
        );
DiscreteDirectionalWaveSpectrum common(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
        )
{
    DiscreteDirectionalWaveSpectrum ret;
    ret.omega = S.get_angular_frequencies(omega_min, omega_max, nfreq);
    ret.psi = D.get_directions(nfreq);
    if (ret.omega.size()>1) ret.domega = ret.omega[1]-ret.omega[0];
    if (ret.psi.size()>1)   ret.dpsi = ret.psi[1]-ret.psi[0];
    ret.Si.reserve(ret.omega.size());
    ret.Dj.reserve(ret.psi.size());
    BOOST_FOREACH(double omega, ret.omega) ret.Si.push_back(S(omega));
    BOOST_FOREACH(double psi, ret.psi)   ret.Dj.push_back(D(psi));
    return ret;
}

DiscreteDirectionalWaveSpectrum discretize(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq                 //!< Number of frequencies & number of directions in discrete spectrum
        )
{
    DiscreteDirectionalWaveSpectrum ret = common(S,D,omega_min,omega_max,nfreq);
    ret.k.reserve(ret.omega.size());
    BOOST_FOREACH(double omega, ret.omega) ret.k.push_back(S.get_wave_number(omega));
    ret.pdyn_factor = [](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,eta);};
    ret.pdyn_factor_sh = [](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,eta);};
    return ret;
}

/**  \author cec
  *  \date Aug 1, 2014, 5:04:24 PM
  *  \brief Discretize a wave spectrum
  *  \details Finite depth hypothesis (for the wave number)
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest discretize_example
  */
DiscreteDirectionalWaveSpectrum discretize(
        const WaveSpectralDensity& S,      //!< Frequency spectrum
        const WaveDirectionalSpreading& D, //!< Spatial spectrum
        const double omega_min,            //!< Upper bound of the angular frequency range (in rad/s)
        const double omega_max,            //!< Upper bound of the angular frequency range (in rad/s)
        const size_t nfreq,                //!< Number of frequencies & number of directions in discrete spectrum
        const double h                     //!< Water depth (in meters)
        )
{
    DiscreteDirectionalWaveSpectrum ret = common(S,D,omega_min,omega_max,nfreq);
    ret.k.reserve(ret.omega.size());
    BOOST_FOREACH(double omega, ret.omega) ret.k.push_back(S.get_wave_number(omega,h));
    ret.pdyn_factor = [h](const double k, const double z, const double eta){return dynamic_pressure_factor(k,z,h,eta);};
    ret.pdyn_factor_sh = [h](const double k, const double z, const double eta){return dynamic_pressure_factor_sh(k,z,h,eta);};
    return ret;
}

typedef std::pair<double,size_t> ValIdx;
bool comparator ( const ValIdx& l, const ValIdx& r);
bool comparator ( const ValIdx& l, const ValIdx& r)
   { return l.first > r.first; }


/**  \brief Only select the most important spectrum components & create single vector
  *  \details No need to loop on all frequencies & all directions: we only select
  *  the most important ones (i.e. those representing a given ratio of the total
  *  energy in the spectrum.
  *  \returns A flat spectrum (i.e. one where the freq & direct. loops have been unrolled)
  *  \snippet environment_models/unit_tests/src/discretizeTest.cpp discretizeTest flatten_example
  */
FlatDiscreteDirectionalWaveSpectrum flatten(
        const DiscreteDirectionalWaveSpectrum& spectrum, //!< Spectrum to flatten
        const double ratio //!< Between 0 & 1: where should we cut off the spectra?
        )
{
    FlatDiscreteDirectionalWaveSpectrum ret;
    ret.domega = spectrum.domega;
    ret.dpsi = spectrum.dpsi;
    double S = 0;
    const size_t nOmega = spectrum.omega.size();
    const size_t nPsi = spectrum.psi.size();
    std::list<ValIdx> SiDj;
    std::vector<size_t> i_idx(nOmega*nPsi);
    std::vector<size_t> j_idx(nOmega*nPsi);
    size_t k = 0;
    for (size_t i = 0 ; i < nOmega ; ++i)
    {
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            const double s = spectrum.Si[i]*spectrum.Dj[j];
            S += s;
            SiDj.push_back(std::make_pair(s,k));
            i_idx[k] = i;
            j_idx[k] = j;
            k++;
        }
    }
    SiDj.sort(comparator);

    double cumsum = 0;
    const double max_energy = ratio*S;
    const size_t n = SiDj.size();
    for (size_t k = 0 ; k < n ; ++k)
    {
        const ValIdx sidj = SiDj.front();
        SiDj.pop_front();
        cumsum += sidj.first;
        if (cumsum>max_energy) return ret;
        DiscreteDirectionalWaveDensity x;
        const size_t i = i_idx[sidj.second];
        const size_t j = j_idx[sidj.second];
        x.k = spectrum.k[i];
        x.omega = spectrum.omega[i];
        x.psi = spectrum.psi[j];
        x.sqrt_2_SiDj = sqrt(2*sidj.first);
        ret.spectrum.push_back(x);
    }
    return ret;
}

double dynamic_pressure_factor(const double k,  //!< Wave number (in 1/m)
                               const double z,  //!< z-position in the NED frame (in meters)
                               const double  //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                              )
{
    return exp(-k*z);
}

double dynamic_pressure_factor(const double k,  //!< Wave number (in 1/m)
                               const double z,  //!< z-position in the NED frame (in meters)
                               const double h,  //!< Average water depth (in meters)
                               const double  //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                              )
{
    return cosh(k*(h-z))/cosh(k*h);
}

double dynamic_pressure_factor_sh(const double k,  //!< Wave number (in 1/m)
                                  const double z,  //!< z-position in the NED frame (in meters)
                                  const double h,  //!< Average water depth (in meters)
                                  const double  //!< Wave elevation at (x,y) in the NED frame (in meters) for stretching
                              )
{
    return sinh(k*(h-z))/cosh(k*h);
}
