/*
 * Sim.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIM_HPP_
#define SIM_HPP_

#include <vector>
#include "Body.hpp"
#include "EnvironmentAndFrames.hpp"
#include "StateMacros.hpp"
#include "UnsafeWrench.hpp"

class Sim
{
    public:
        Sim(const std::vector<Body>& bodies,
            const std::vector<ListOfForces>& forces,
            const KinematicsPtr& k,
            const StateType& x);
        void operator()(const StateType& x, StateType& dxdt, double t);

        void update_discrete_states();
        void update_continuous_states();
        StateType get_state_derivatives() const;
        std::vector<std::string> get_names_of_bodies() const;

        StateType state;

    private:
        UnsafeWrench sum_of_forces(const StateType& x, const size_t body_index) const;
        void calculate_state_derivatives(const Wrench& sum_of_forces,
                                         const MatrixPtr& inverse_of_the_total_inertia,
                                         const StateType& x,
                                         StateType& dx_dt,
                                         const size_t index_of_first_state) const;

        std::vector<Body> bodies;
        std::vector<ListOfForces> forces;
        KinematicsPtr k;
        StateType _dx_dt;
};

#endif /* SIM_HPP_ */
