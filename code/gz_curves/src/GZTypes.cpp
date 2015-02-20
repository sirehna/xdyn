/*
 * GZTypes.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "GZTypes.hpp"


GZ::State::State(const double z_, const double phi_, const double theta_) :
    z(z_),
    phi(phi_),
    theta(theta_)
{
}

GZ::Resultant::Resultant() : state(0,0,0), centre_of_buyoancy()
{
}
