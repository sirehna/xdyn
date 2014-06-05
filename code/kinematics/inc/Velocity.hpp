/*
 * Velocity.hpp
 *
 *  Created on: 7 avr. 2014
 *      Author: maroff
 */

#ifndef VELOCITY_HPP_
#define VELOCITY_HPP_

#include "Point.hpp"

typedef Point AngularVelocityVector;
typedef Point TranslationVelocityVector;


/** \author cec
 *  \brief
 *  \details
 *  \ingroup kinematics
 */
class Velocity
{
    public:
        Velocity(const Point& p, const AngularVelocityVector& w);
        Velocity(const Point& p, const TranslationVelocityVector& t, const AngularVelocityVector& w);
        Velocity& operator=(const Velocity& rhs);
        Velocity(const Velocity& rhs);
        std::string get_frame() const;
        Point get_point() const;
        AngularVelocityVector get_angular_velocity() const;
        TranslationVelocityVector get_translation_velocity() const;

    private:
        Point P;
        TranslationVelocityVector vP;
        AngularVelocityVector omega;

    public:
        double& u;
        double& v;
        double& w;
        double& p;
        double& q;
        double& r;

        Velocity change_point(const Point& P) const;


    private:
        Velocity();
};

#endif // VELOCITY_HPP_
