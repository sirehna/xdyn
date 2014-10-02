/*
 * DampingForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: jacquenot
 */

#ifndef DampingForceModel_HPP_
#define DampingForceModel_HPP_

#include "ForceModel.hpp"
#include <ssc/kinematics.hpp>

class Body;

class DampingForceModel : public ForceModel
{
    public:
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;
};

#endif /* DampingForceModel_HPP_ */
