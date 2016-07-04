/*
 * Airy.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "Airy.hpp"

Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const double constant_random_phase) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t nPsi = spectrum.psi.size();
    const size_t nOmega = spectrum.omega.size();
    spectrum.phase = std::vector<std::vector<double> >(nOmega, std::vector<double>(nPsi, constant_random_phase));
}

Airy::Airy(const DiscreteDirectionalWaveSpectrum& spectrum_, const int random_number_generator_seed) : WaveModel(spectrum_),
phase(std::vector<std::vector<double> >()),
rng(boost::mt19937((unsigned int)random_number_generator_seed)),
generate_random_phase(boost::random::uniform_real_distribution<double>(0,2*PI))
{
    const size_t nPsi = spectrum.psi.size();
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        std::vector<double> p(nPsi,0.0);
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            p[j] = generate_random_phase(rng);
        }
        spectrum.phase.push_back(p);
    }
}

double Airy::evaluate_rao(
        const double x,                                         //!< x-position of the RAO's calculation point in the NED frame (in meters)
        const double y,                                         //!< y-position of the RAO's calculation point in the NED frame (in meters)
        const double t,                                         //!< Current time instant (in seconds)
        const std::vector<std::vector<double> >& rao_module,    //!< Module of the RAO
        const std::vector<std::vector<double> >& rao_phase      //!< Phase of the RAO
        ) const
{
    double F = 0;
    const size_t nPsi = spectrum.psi.size();
    std::vector<double> v_Dj(nPsi), v_xCosPsi_ySinPsi(nPsi);
    for (size_t j = 0 ; j < nPsi ; ++j)
    {
        const double psi = spectrum.psi[j];
        v_xCosPsi_ySinPsi[j] = x*cos(psi)+y*sin(psi);
        v_Dj[j] = sqrt(spectrum.Dj[j]);
    }
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        const double k = spectrum.k[i];
        const double omega_t = spectrum.omega[i] * t;
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            const double Aij = rao_module[i][j] * sqrt(spectrum.Si[i]);
            const double Dj = v_Dj[j];
            const double k_xCosPsi_ySinPsi = k * v_xCosPsi_ySinPsi[j];
            const double theta = rao_phase[i][j];
            F += Aij*Dj*cos(omega_t - k_xCosPsi_ySinPsi + theta + spectrum.phase[i][j]);
        }
    }
    F *= sqrt(2*spectrum.domega*spectrum.dpsi);
    return F;
}

double Airy::elevation(
        const double x,     //!< x-position in the NED frame (in meters)
        const double y,     //!< y-position in the NED frame (in meters)
        const double t      //!< Current time instant (in seconds)
        ) const
{
    double zeta = 0;
    const size_t nPsi = spectrum.psi.size();
    std::vector<double> v_Dj(nPsi), v_xCosPsi_ySinPsi(nPsi);
    for (size_t j = 0 ; j < nPsi ; ++j)
    {
        const double psi = spectrum.psi[j];
        v_xCosPsi_ySinPsi[j] = x*cos(psi)+y*sin(psi);
        v_Dj[j] = sqrt(spectrum.Dj[j]);
    }
    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        const double Ai = sqrt(spectrum.Si[i]);
        const double k = spectrum.k[i];
        const double omega_t = spectrum.omega[i]*t;
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            const double Dj = v_Dj[j];
            const double k_xCosPsi_ySinPsi = k * v_xCosPsi_ySinPsi[j];
            const double theta = spectrum.phase[i][j];
            zeta += Ai * Dj * cos(omega_t - k_xCosPsi_ySinPsi + theta);
        }
    }
    zeta *= sqrt(2*spectrum.domega*spectrum.dpsi);
    return zeta;
}

double Airy::dynamic_pressure(
        const double rho, //!< water density (in kg/m^3)
        const double g,   //!< gravity (in m/s^2)
        const double x,   //!< x-position in the NED frame (in meters)
        const double y,   //!< y-position in the NED frame (in meters)
        const double z,   //!< z-position in the NED frame (in meters)
        const double eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
        const double t    //!< Current time instant (in seconds)
        ) const
{
    double p = 0;

    const size_t nPsi = spectrum.psi.size();
    std::vector<double> v_Dj(nPsi), v_xCosPsi_ySinPsi(nPsi);
    for (size_t j = 0 ; j < nPsi ; ++j)
    {
        const double psi = spectrum.psi[j];
        v_xCosPsi_ySinPsi[j] = x*cos(psi)+y*sin(psi);
        v_Dj[j] = sqrt(spectrum.Dj[j]);
    }

    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        const double Ai = sqrt(spectrum.Si[i]);
        const double k = spectrum.k[i];
        const double omega_t = spectrum.omega[i] * t;
        const double pdyn_fact = spectrum.pdyn_factor(k,z,eta);
        for (size_t j = 0 ; j < spectrum.psi.size() ; ++j)
        {
            const double Dj = v_Dj[j];
            const double k_xCosPsi_ySinPsi = k * v_xCosPsi_ySinPsi[j];
            const double theta = spectrum.phase[i][j];
            p += Ai * Dj * pdyn_fact * cos(omega_t - k_xCosPsi_ySinPsi + theta);
        }
    }
    p *= rho*g*sqrt(2*spectrum.domega*spectrum.dpsi);
    return p;
}

ssc::kinematics::Point Airy::orbital_velocity(
        const double g,   //!< gravity (in m/s^2)
        const double x,   //!< x-position in the NED frame (in meters)
        const double y,   //!< y-position in the NED frame (in meters)
        const double z,   //!< z-position in the NED frame (in meters)
        const double t    //!< Current time instant (in seconds)
        ) const
{
    double u = 0;
    double v = 0;
    double w = 0;
    const double eta = 0; // No stretching for the orbital velocity

    const size_t nPsi = spectrum.psi.size();
    std::vector<double> v_Dj(nPsi), v_xCosPsi_ySinPsi(nPsi);
    std::vector<double> v_CosPsi(nPsi), v_SinPsi(nPsi);
    for (size_t j = 0 ; j < nPsi ; ++j)
    {
        const double psi = spectrum.psi[j];
        const double cosPsi = v_CosPsi[j] = cos(psi);
        const double sinPsi = v_SinPsi[j] = sin(psi);
        v_xCosPsi_ySinPsi[j] = x*cosPsi+y*sinPsi;
        v_Dj[j] = sqrt(spectrum.Dj[j]);
    }

    for (size_t i = 0 ; i < spectrum.omega.size() ; ++i)
    {
        const double Ai = sqrt(spectrum.Si[i]);
        const double omega = spectrum.omega[i];
        const double k = spectrum.k[i];
        const double pdyn_factor = spectrum.pdyn_factor(k,z,eta);
        const double pdyn_factor_sh = spectrum.pdyn_factor_sh(k,z,eta);
        for (size_t j = 0 ; j < nPsi ; ++j)
        {
            const double theta = omega*t-k*v_xCosPsi_ySinPsi[j] + spectrum.phase[i][j];
            const double cos_theta = cos(theta);
            const double sin_theta = sin(theta);
            const double Ai_Dj_k_omega = Ai*v_Dj[j]*k/omega;
            const double Ai_Dj_k_omega_pdyn_factor_cos_theta = Ai_Dj_k_omega*pdyn_factor*cos_theta;
            u += Ai_Dj_k_omega_pdyn_factor_cos_theta * v_CosPsi[j];
            v += Ai_Dj_k_omega_pdyn_factor_cos_theta * v_SinPsi[j];
            w += Ai_Dj_k_omega * pdyn_factor_sh * sin_theta;
        }
    }
    const double f = g*sqrt(2*spectrum.domega*spectrum.dpsi);
    u *= f;
    v *= f;
    w *= f;
    return ssc::kinematics::Point("NED", u, v, w);
}
