/*
 * SimHdf5Observer.hpp
 *
 *  Created on: Dec 15, 2014
 *      Author: jacquenot
 */

#ifndef SIMHDF5OBSERVER_HPP_
#define SIMHDF5OBSERVER_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)
#include <ssc/kinematics.hpp>

#include <string>
#include <vector>

class Sim;

class SimHdf5Observer
{
    public:

        SimHdf5Observer(const std::string& fileName, const Sim& s);
        SimHdf5Observer(const std::string& fileName, const std::string& baseName, const Sim& s);
        void observe(const Sim& sys, const double t);
        void observe_states(const Sim& sys, const double t);
        void observe_efforts(const Sim& sys, const double t);


    private:
        SimHdf5Observer(); // Disabled
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* SIMHDF5OBSERVER_HPP_ */
