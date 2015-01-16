/*
 * Body.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include "BodyStates.hpp"
#include "StateMacros.hpp"
struct YamlBody;
typedef TR1(shared_ptr)<ssc::kinematics::Kinematics> KinematicsPtr;
struct YamlRotation;

class Observer;

class Body
{
    public:
        virtual ~Body();
        Body(const size_t idx);
        Body(const BodyStates& states, const size_t idx);

        BodyStates get_states() const;

        /** \brief Use SurfaceElevation to compute wave height & update accordingly
         */
        virtual void update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                                   const double t
                                                  ) = 0;

        ssc::kinematics::Point get_origin(const StateType& x) const;
        ssc::kinematics::Point get_position_of_body_relative_to_mesh() const;
        ssc::kinematics::Transform get_transform_from_mesh_to() const;
        ssc::kinematics::Transform get_transform_from_ned_to(const StateType& x) const;
        /**  \brief Update Body structure taking the new coordinates & wave heights into account
         */
        void update(const EnvironmentAndFrames& env, const StateType& x, const double t);
        void update_kinematics(StateType x, const KinematicsPtr& k) const;
        void update_body_states(const StateType& x);
        /**  \brief Update down vector (expressed in body's mesh frame), taking the new coordinates into account
         */
        void update_projection_of_z_in_mesh_frame(const double g,
                                                  const ssc::kinematics::KinematicsPtr& k);

        void calculate_state_derivatives(const ssc::kinematics::Wrench& sum_of_forces,
                                         const StateType& x,
                                         StateType& dx_dt,
                                         const EnvironmentAndFrames& env) const;

        Eigen::Vector3d get_uvw_in_body_frame(const StateType& x) const;
        Eigen::Vector3d get_pqr(const StateType& x) const;
        std::string get_name() const;
        ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x) const;
        ssc::kinematics::EulerAngles get_angles(const StateType& all_states, const YamlRotation& c) const;

        void feed(const StateType& x, Observer& observer, const YamlRotation& c) const;

    protected:
        BodyStates states;

    private:
        Body();

        size_t idx; //!< Index of the first state
};

typedef TR1(shared_ptr)<Body> BodyPtr;

#endif /* BODY_HPP_ */
