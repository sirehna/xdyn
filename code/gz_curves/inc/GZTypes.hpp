/*
 * GZTypes.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#ifndef GZTYPES_HPP_
#define GZTYPES_HPP_

#include <ssc/kinematics.hpp>

namespace GZ
{
    typedef Eigen::Vector3d State;

    struct Resultant
    {
        Resultant();
        State state;
        ssc::kinematics::Point centre_of_buyoancy;
    };
}

#endif /* GZTYPES_HPP_ */
