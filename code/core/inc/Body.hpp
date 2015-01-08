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

class Body
{
    public:
        Body(const size_t idx);
        /** \brief Use SurfaceElevation to compute wave height & update accordingly
         */
        void update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                                   const double t
                                                  );

        ssc::kinematics::Point get_origin(const StateType& x) const;
        ssc::kinematics::RotationMatrix get_rot_from_ned_to(const StateType& x) const;
        ssc::kinematics::Point get_position_of_body_relative_to_mesh() const;
        ssc::kinematics::Transform get_transform_from_mesh_to() const;
        ssc::kinematics::Transform get_transform_from_ned_to(const StateType& x) const;
        void update_kinematics(StateType x, const KinematicsPtr& k) const;
        void update_body_states(const StateType& x);


        BodyStates states;

    private:
        Body();
        size_t idx;
};

#endif /* BODY_HPP_ */
