/*
 * ResultantForceComputer.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "ResultantForceComputer.hpp"
#include "GZTypes.hpp"

GZ::ResultantForceComputer::ResultantForceComputer(const Sim& )
{

}

GZ::State GZ::ResultantForceComputer::operator()(const ::GZ::State& point) const
{
    return point;
}

GZ::ResultantForceComputer::~ResultantForceComputer()
{
}
