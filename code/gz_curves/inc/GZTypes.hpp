/*
 * GZTypes.hpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#ifndef GZTYPES_HPP_
#define GZTYPES_HPP_

namespace GZ
{
    struct State
    {
        State(const double z, const double phi, const double theta);
        double z;
        double phi;
        double theta;
    };
}

#endif /* GZTYPES_HPP_ */
