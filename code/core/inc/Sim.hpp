/*
 * Sim.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef SIM_HPP_
#define SIM_HPP_

#include <vector>
#include <ssc/data_source.hpp>
#include <ssc/kinematics.hpp>
#include "Body.hpp"
#include "StateMacros.hpp"
#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"
#include "ControllableForceModel.hpp"

typedef std::map<std::string, std::map< std::string,ssc::kinematics::Vector6d > > OuputtedForces;
typedef std::vector<std::pair<std::string,std::vector<std::string> > > VectorOfStringModelForEachBody;

class Sim
{
    public:
        Sim(const std::vector<BodyPtr>& bodies,
            const std::vector<ListOfForces>& forces,
            const std::vector<ListOfControlledForces>& controllable_forces,
            const EnvironmentAndFrames& env,
            const StateType& x,
            const ssc::data_source::DataSource& command_listener);
        void operator()(const StateType& x, StateType& dxdt, double t);

        void update_discrete_states();
        void update_continuous_states();
        StateType get_state_derivatives() const;
        std::vector<std::string> get_names_of_bodies() const;
        std::vector<std::string> get_force_names() const;
        VectorOfStringModelForEachBody get_vector_of_string_model_for_each_body() const;
        OuputtedForces get_forces() const;
        std::vector<double> get_forces_as_a_vector_of_doubles() const;

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
        ssc::kinematics::UnsafeWrench sum_of_forces(const StateType& x, const BodyPtr& body, const double t);

        /**  \brief Make sure quaternions can be converted to Euler angles
          *  \details Normalization takes place at each time step, which is not
          *  ideal because it means the model does not see the state values set
          *  by the stepper.
          */
        StateType normalize_quaternions(const StateType& all_states
                                       ) const;

        void fill_force(OuputtedForces& ret, const std::string& body_name, const std::string& force_name, const ssc::kinematics::Wrench& tau) const;
        void fill_force_map_with_zeros(OuputtedForces& m) const;

        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};

#endif /* SIM_HPP_ */
