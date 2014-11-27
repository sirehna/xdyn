/*
 * HDBData.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include "HDBBuilder.hpp"
#include "HDBData.hpp"
#include <ssc/interpolation.hpp>

class HDBData::Impl
{
    public:
        Impl(const HDBBuilder& builder) : M(), Tmin(0)
        {
            bool allow_queries_outside_bounds;
            const TimestampedMatrices Ma = builder.get_added_mass();
            const auto x = get_Tp(Ma);
            Tmin = x.front();
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    M[i][j] = ssc::interpolation::SplineVariableStep(x, get_Mij_for_each_Tp(Ma,i,j), allow_queries_outside_bounds=true);
                }
            }

            const TimestampedMatrices Br = builder.get_radiation_damping();
            T_rad = get_Tp(Br);
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

        std::vector<double> T_rad;

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
        double Tmin;
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

std::vector<double> HDBData::get_radiation_damping_periods() const
{
    return pimpl->T_rad;

}
