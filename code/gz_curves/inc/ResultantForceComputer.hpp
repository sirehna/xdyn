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
    class ResultantForceComputer
    {
        public:
            ResultantForceComputer(const Sim& sim);
            ::GZ::Resultant operator()(const ::GZ::State& point);
            double gz(const ssc::kinematics::Point& B //!< Centre of buoyancy in the body frame
                     ) const;

        private:
            BodyPtr body;
            EnvironmentAndFrames env;
            TR1(shared_ptr)<GravityForceModel> gravity;
            TR1(shared_ptr)<HydrostaticForceModel> hydrostatic;
            double current_instant;
            ssc::kinematics::Point G;
    };
}

#endif /* RESULTANTFORCECOMPUTER_HPP_ */
