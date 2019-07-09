/*
 * Res.cpp
 *
 *  Created on: Jul 1, 2014
 *      Author: cady
 */

#include "Res.hpp"

Res::Res() : t(0), x(), extra_observations()
{
}

Res::Res(const double t_, const StateType& x_) : t(t_), x(x_), extra_observations()
{
}

