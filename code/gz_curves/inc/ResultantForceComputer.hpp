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

namespace GZ
{
    struct State;
    class ResultantForceComputer
    {
        public:
            ResultantForceComputer(const Sim& sim);
            ::GZ::State operator()(const ::GZ::State& point);
            ~ResultantForceComputer();

        private:
            BodyPtr body;
            EnvironmentAndFrames env;
            ForcePtr gravity;
            ForcePtr hydrostatic;
            double current_instant;
    };
}

#endif /* RESULTANTFORCECOMPUTER_HPP_ */
