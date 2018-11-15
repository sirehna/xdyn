#include "ConfBuilder.hpp"
#include "Res.hpp"
#include "Sim.hpp"
#include "State.hpp"

#ifndef OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_
#define OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_

class SimServerInputs;

class SimStepper
{
    public:
        SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt);
        std::vector<Res> step(const SimServerInputs& input, double Dt);


    private:
        Sim sim;
        const std::string solver;
        const double dt;
};

#endif /* OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_ */
