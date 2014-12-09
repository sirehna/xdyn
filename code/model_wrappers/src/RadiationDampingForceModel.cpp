/*
 * DampingForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#include <array>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include "Body.hpp"
#include "HDBParser.hpp"
#include "History.hpp"
#include "RadiationDampingBuilder.hpp"
#include "RadiationDampingForceModel.hpp"
#include "RadiationDampingForceModelException.hpp"

#include <ssc/macros.hpp>
#include <cassert>
#include <sstream>

class RadiationDampingForceModel::Impl
{
    public:
        Impl(const TR1(shared_ptr)<HDBParser>& parser, const YamlRadiationDamping& yaml) : hdb{parser}, h{{History(1),History(1),History(1),History(1),History(1),History(1)}}, builder(RadiationDampingBuilder(yaml.interpolation, yaml.quadrature)), K(),
        omega(parser->get_radiation_damping_angular_frequencies()), taus(), n(yaml.nb_of_points_in_retardation_function), Tmin(0), Tmax(0)
        {
            taus = builder.build_regular_intervals(2*PI/omega.back(),2*PI/omega.front(),n);
            if (not(omega.empty()))
            {
                Tmin = 2*PI/omega.back();
                Tmax = 2*PI/omega.front();
                if (Tmin>Tmax)
                {
                    std::stringstream ss;
                    ss << "Vector of angular frequencies is not in strictly increasing order: omega[0]="<<omega[0] << ", but omega[" << omega.size()-1 << "]=" << omega.back();
                    THROW(__PRETTY_FUNCTION__, RadiationDampingForceModelException, ss.str());
                }
            }
            for (size_t i = 0 ; i < 6 ; ++i)
            {
                for (size_t j = 0 ; j < 6 ; ++j)
                {
                    K[i][j] = get_K(i, j);
                }
                h.at(i) = History(Tmax);
            }
        }

        std::function<double(double)> get_K(const size_t i, const size_t j) const
        {
            const auto Br__ = builder.build_interpolator(omega,hdb->get_radiation_damping_coeff(i,j));
            const auto K  = builder.build_retardation_function(Br__,taus,1E-3);
            return K;
        }

        double get_convolution_for_axis(const size_t i) const
        {
            double K_X_dot = 0;
            for (size_t k = 0 ; k < 6 ; ++k)
            {
                if (h[k].get_length() >= Tmin)
                {
                    // Integrate up to Tmax if possible, but never exceed the history length
                    if (i==0)
                    {
                        std::cout.precision(20);
                        COUT(Tmin);
                        COUT(std::min(Tmax, h[k].get_length()));
                    }
                    const double co = builder.convolution(h[k], K[i][k], Tmin, std::min(Tmax, h[k].get_length()));
                    K_X_dot += co;
                }
            }
            return K_X_dot;
        }

        ssc::kinematics::Wrench get_wrench(const Body& b, const double t)
        {
            save_state(b, t);
            ssc::kinematics::Wrench ret(b.name);
            ret.X() = get_convolution_for_axis(0);
            ret.Y() = get_convolution_for_axis(1);
            ret.Z() = get_convolution_for_axis(2);
            ret.K() = get_convolution_for_axis(3);
            ret.M() = get_convolution_for_axis(4);
            ret.N() = get_convolution_for_axis(5);
            return ret;
        }

        void save_state(const Body& b, const double t)
        {
            h[0].record(t,b.u);
            h[1].record(t,b.v);
            h[2].record(t,b.w);
            h[3].record(t,b.p);
            h[4].record(t,b.q);
            h[5].record(t,b.r);
        }

    private:
        Impl();
        TR1(shared_ptr)<HDBParser> hdb;
        std::array<History, 6> h;
        RadiationDampingBuilder builder;
        std::array<std::array<std::function<double(double)>,6>, 6> K;
        std::vector<double> omega;
        std::vector<double> taus;
        size_t n;
        double Tmin;
        double Tmax;
};


RadiationDampingForceModel::RadiationDampingForceModel(const TR1(shared_ptr)<HDBParser>& parser, const YamlRadiationDamping& yaml) : ForceModel("radiation damping"),
pimpl(new Impl(parser, yaml))
{
}

ssc::kinematics::Wrench RadiationDampingForceModel::operator()(const Body& b, const double t) const
{
    return pimpl->get_wrench(b, t);
}
