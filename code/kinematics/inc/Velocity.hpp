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


/** \author cec
 *  \brief 
 *  \details 
 *  \ingroup kinematics
 */
class Velocity
{
    public:
        Velocity(const Point& p, const AngularVelocityVector& w);

    private:
        Point P;
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
