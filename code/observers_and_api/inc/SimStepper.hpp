#include "Sim.hpp"
#include "State.hpp"
#include "ConfBuilder.hpp"

#ifndef OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_
#define OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_

class SimStepper
{
    public:
        SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt);
        State step(const State& state, const double t, const double Dt);


    private:
        Sim sim;
        const std::string solver;
        const double dt;
};

#endif /* OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_ */
