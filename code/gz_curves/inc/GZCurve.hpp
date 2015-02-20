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

class Sim;

namespace GZ
{
    class Curve
    {
        public:
            Curve(const Sim& sim);
            static std::vector<double> get_phi(const double dphi, const double phi_max);


        private:
            Curve();
    };
}

#endif /* GZCURVE_HPP_ */
