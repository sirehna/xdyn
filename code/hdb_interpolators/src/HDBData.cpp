/*
 * HDBData.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "HDBBuilder.hpp"
#include "HDBBuilderException.hpp"
#include "HDBData.hpp"
#include <ssc/interpolation.hpp>

#include <ssc/macros.hpp>

class HDBData::Impl
{
    public:
        Impl(const HDBBuilder& builder) : omega_rad(), M(), Br(), Tmin(0), module(builder.get_diffraction_module()), phase(builder.get_diffraction_phase())
        {
            bool allow_queries_outside_bounds;
            const TimestampedMatrices Ma = builder.get_added_mass();
            const TimestampedMatrices B_r = builder.get_radiation_damping();
            const auto x = get_Tp(Ma);
            Tmin = x.front();
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    M[i][j] = ssc::interpolation::SplineVariableStep(x, get_Mij_for_each_Tp(Ma,i,j), allow_queries_outside_bounds=true);
                    Br[i][j] = get_Mij_for_each_Tp(B_r, i, j);
                }
            }


            auto v = get_Tp(B_r);
            for (auto it = v.rbegin(); it != v.rend() ; ++it)
            {
                if(*it==0)
                {
                    THROW(__PRETTY_FUNCTION__, HDBBuilderException, "Zero period detected: cannot compute angular frequency. Check Added_mass_Radiation_Damping section of the HDB file.");
                }
                omega_rad.push_back(2*PI/ *it);
            }
        }

        std::array<std::vector<std::vector<double> >,6 > get_diffraction_module_tables() const
        {
            return module.values;
        }

        std::array<std::vector<std::vector<double> >,6 > get_diffraction_phase_tables() const
        {
            return phase.values;
        }

        Eigen::Matrix<double,6,6> get_added_mass(const double Tp)
        {
            Eigen::Matrix<double,6,6> ret;
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    ret((int)i,(int)j) = M[i][j].f(Tp);
                }
            }
            return ret;
        }

        Eigen::Matrix<double,6,6> get_added_mass()
        {
            return get_added_mass(Tmin);
        }

        std::vector<double> get_radiation_damping_coeff(const size_t i, const size_t j) const
        {
            const auto v = Br[i][j];
            return std::vector<double>(v.rbegin(), v.rend());
        }

        std::vector<double> get_radiation_phase_psis() const
        {
            return phase.psi;
        }

        std::vector<double> get_radiation_phase_omegas() const
        {
            return phase.omega;
        }

        std::vector<double> get_radiation_module_psis() const
        {
            return module.psi;
        }

        std::vector<double> get_radiation_module_omegas() const
        {
            return module.omega;
        }

        std::vector<double> omega_rad;

    private:
        std::vector<double> get_Tp(const TimestampedMatrices& M)
        {
            std::vector<double> ret;
            for (size_t k = 0 ; k < M.size() ; ++k) ret.push_back(M.at(k).first);
            return ret;
        }

        std::vector<double> get_Mij_for_each_Tp(const TimestampedMatrices& M, const size_t i, const size_t j)
        {
            std::vector<double> ret;
            for (size_t k = 0 ; k < M.size() ; ++k) ret.push_back(M.at(k).second.at(i).at(j));
            return ret;
        }

        std::array<std::array<ssc::interpolation::SplineVariableStep,6>,6> M;
        std::array<std::array<std::vector<double>,6>,6> Br;
        double Tmin;
        RAOData module;
        RAOData phase;
};


HDBData::HDBData(const HDBBuilder& builder) : pimpl(new HDBData::Impl(builder))
{
}

Eigen::Matrix<double,6,6> HDBData::get_added_mass() const
{
    return pimpl->get_added_mass();
}

Eigen::Matrix<double,6,6> HDBData::get_added_mass(const double Tp //!< Period at which to interpolate the added mass
                                                 ) const
{
    return pimpl->get_added_mass(Tp);
}

std::vector<double> HDBData::get_radiation_damping_angular_frequencies() const
{
    return pimpl->omega_rad;

}

std::vector<double> HDBData::get_radiation_damping_coeff(const size_t i, const size_t j) const
{
    return pimpl->get_radiation_damping_coeff(i, j);
}

std::array<std::vector<std::vector<double> >,6 > HDBData::get_diffraction_module_tables() const
{
    return pimpl->get_diffraction_module_tables();
}

std::array<std::vector<std::vector<double> >,6 > HDBData::get_diffraction_phase_tables() const
{
    return pimpl->get_diffraction_phase_tables();
}

std::vector<double> HDBData::get_radiation_phase_psis() const
{
    return pimpl->get_radiation_phase_psis();
}

std::vector<double> HDBData::get_radiation_phase_omegas() const
{
    return pimpl->get_radiation_phase_omegas();
}

std::vector<double> HDBData::get_radiation_module_psis() const
{
    return pimpl->get_radiation_module_psis();
}

std::vector<double> HDBData::get_radiation_module_omegas() const
{
    return pimpl->get_radiation_module_omegas();
}
