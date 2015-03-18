/*
 * GZCurve.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */

#ifndef GZCURVE_HPP_
#define GZCURVE_HPP_

#include <string>
#include <vector>

#include <ssc/macros/tr1_macros.hpp>

#include TR1INC(memory)

#include "GZTypes.hpp"

class Sim;

namespace GZ
{
    class Curve
    {
        public:
            Curve(const Sim& sim);
            static std::vector<double> get_phi(const double dphi, const double phi_max);
            double gz(const double phi) const;
            double zeq(const double phi, const double theta) const;
            double get_theta_eq() const;
            State get_Xeq() const;

        private:
            Curve();
            struct Impl;
            TR1(shared_ptr)<Impl> pimpl;
            double theta_eq;
    };
}

#endif /* GZCURVE_HPP_ */
