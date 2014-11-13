/*
 * HDBData.cpp
 *
 *  Created on: Nov 13, 2014
 *      Author: cady
 */

#include <ssc/interpolation.hpp>

#include "HDBData.hpp"


class HDBData::Impl
{
    public:
        Impl(const TimestampedMatrices& Ma) : M()
        {
            const auto x = get_Tp(Ma);
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    M[i][j] = ssc::interpolation::SplineVariableStep(x, get_Mij_for_each_Tp(Ma,i,j));
                }
            }
        }

        Eigen::Matrix<double,6,6> get_added_mass(const double Tp)
        {
            Eigen::Matrix<double,6,6> ret;
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    ret(i,j) = M[i][j].f(Tp);
                }
            }
            return ret;
        }

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
};


HDBData::HDBData(const TimestampedMatrices& Ma) : pimpl(new HDBData::Impl(Ma))
{
}

Eigen::Matrix<double,6,6> HDBData::get_added_mass(const double Tp //!< Period at which to interpolate the added mass
                                                 ) const
{
    return pimpl->get_added_mass(Tp);
}
