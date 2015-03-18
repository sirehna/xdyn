/*
 * ResultantForceComputer.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#ifndef RESULTANTFORCECOMPUTER_HPP_
#define RESULTANTFORCECOMPUTER_HPP_

#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"
#include "GZTypes.hpp"

class Sim;
class GravityForceModel;
class HydrostaticForceModel;

namespace GZ
{
    struct MinMax
    {
        MinMax(const double min_, const double max_) : min(min_), max(max_) {}
        double min;
        double max;
    };

    class ResultantForceComputer
    {
        public:
            ResultantForceComputer(const Sim& sim, const double dz = 1e-6, const double dphi = 1e-6, const double dtheta = 1e-6);
            ::GZ::Resultant resultant(const ::GZ::State& point);
            double gz(const ssc::kinematics::Point& B //!< Centre of buoyancy in the body frame
                     ) const;
            Eigen::Matrix2d K(const Eigen::Vector3d& X);

            MinMax get_zmin_zmax(const double phi);

        private:
            Eigen::Matrix2d dF(const Eigen::Vector3d& X);

            BodyPtr body;
            EnvironmentAndFrames env;
            TR1(shared_ptr)<GravityForceModel> gravity;
            TR1(shared_ptr)<HydrostaticForceModel> hydrostatic;
            double current_instant;
            ssc::kinematics::Point G;
            double dz;
            double dphi;
            double dtheta;
    };
}

std::ostream& operator<<(std::ostream& os, const GZ::MinMax& m);

#endif /* RESULTANTFORCECOMPUTER_HPP_ */
