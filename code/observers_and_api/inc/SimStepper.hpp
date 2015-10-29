#include "Sim.hpp"
#include "State.hpp"
#include "ConfBuilder.hpp"

#ifndef OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_
#define OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_

struct SimStepperInfos
{
    SimStepperInfos();
    double t;
    State state;
    std::map<std::string, double> commands;
};


class SimStepper
{
    public:
        SimStepper(const ConfBuilder& builder, const std::string& solver, const double dt);
        State step(const SimStepperInfos& infos, double Dt);


    private:
        Sim sim;
        const std::string solver;
        const double dt;
};

#endif /* OBSERVERS_AND_API_INC_SIMSTEPPER_HPP_ */
