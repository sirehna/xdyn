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

class Sim;
class GravityForceModel;
class HydrostaticForceModel;

namespace GZ
{
    struct Resultant;
    struct State;
    class ResultantForceComputer
    {
        public:
            ResultantForceComputer(const Sim& sim);
            ::GZ::Resultant operator()(const ::GZ::State& point);

        private:
            BodyPtr body;
            EnvironmentAndFrames env;
            TR1(shared_ptr)<GravityForceModel> gravity;
            TR1(shared_ptr)<HydrostaticForceModel> hydrostatic;
            double current_instant;
    };
}

#endif /* RESULTANTFORCECOMPUTER_HPP_ */
