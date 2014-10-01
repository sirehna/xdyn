/*
 * Sim.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIM_HPP_
#define SIM_HPP_

#include <vector>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "StateMacros.hpp"
#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"

class Sim
{
    public:
        Sim(const std::vector<Body>& bodies,
            const std::vector<ListOfForces>& forces,
            const EnvironmentAndFrames& env,
            const StateType& x);
        void operator()(const StateType& x, StateType& dxdt, double t);

        void update_discrete_states();
        void update_continuous_states();
        StateType get_state_derivatives() const;
        std::vector<std::string> get_names_of_bodies() const;

        /**  \brief Serialize wave data on mesh
          *  \details Called by SimCsvObserver at each time step. The aim is to
          *  calculate the wave data on a mesh expressed in a particular frame of
          *  reference (eg. NED or body). For example we might want to calculate the
          *  wave data on a mesh surrounding the ship for visualization purposes.
          *  \returns Vector of coordinates on the free surface (in the NED frame),
          *           the z coordinate being the wave height (in meters)
          *  \snippet simulator/unit_tests/src/SimTest.cpp SimTest get_waves_example
          */
        std::vector<ssc::kinematics::Point> get_waves(const double t            //!< Current instant
                                                     ) const;

        StateType state;

    private:
        ssc::kinematics::UnsafeWrench sum_of_forces(const StateType& x, const size_t body_index, const double t) const;
        void calculate_state_derivatives(const ssc::kinematics::Wrench& sum_of_forces,
                                         const MatrixPtr& inverse_of_the_total_inertia,
                                         const StateType& x,
                                         StateType& dx_dt,
                                         const size_t index_of_first_state) const;

        /**  \brief Make sure quaternions can be converted to Euler angles
          *  \details Normalization takes place at each time step, which is not
          *  ideal because it means the model does not see the state values set
          *  by the stepper.
          */
        void normalize_quaternions(StateType& all_states, //!< States of all bodies in the system
                                   const size_t i         //!< Index of the body under consideration
                                   );

        /**  \brief Compute relative immersions & update intersector
         */
        void update_intersection_with_free_surface(Body& body,    //!< Body we wish to update
                                                   const double t //!< Current instant
                                                   ) const;

        std::vector<Body> bodies;
        std::vector<ListOfForces> forces;
        EnvironmentAndFrames env;
        StateType _dx_dt;
};

#endif /* SIM_HPP_ */
