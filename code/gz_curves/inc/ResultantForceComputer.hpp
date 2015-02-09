/*
 * ResultantForceComputer.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#ifndef RESULTANTFORCECOMPUTER_HPP_
#define RESULTANTFORCECOMPUTER_HPP_

class Sim;

namespace GZ
{
    struct State;
    class ResultantForceComputer
    {
        public:
            ResultantForceComputer(const Sim& sim);
            ::GZ::State operator()(const ::GZ::State& point) const;
            ~ResultantForceComputer();
    };
}

#endif /* RESULTANTFORCECOMPUTER_HPP_ */
