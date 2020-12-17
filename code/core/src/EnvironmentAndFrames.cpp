/*
 * EnvironmentAndFrames.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "EnvironmentAndFrames.hpp"

#include "InvalidInputException.hpp"
#include "SurfaceElevationInterface.hpp"
#include "Observer.hpp"

#include <ssc/kinematics.hpp>
using namespace ssc::kinematics;

EnvironmentAndFrames::EnvironmentAndFrames() : w(),
                                               k(KinematicsPtr(new Kinematics())),
                                               rho(0),
                                               nu(0),
                                               g(0),
                                               rot(),
											   rho_air()
{
    if (rho<0.0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "rho can not be negative");
    }
    if (rho_air && rho_air<0.0)
    {
    	THROW(__PRETTY_FUNCTION__, InvalidInputException, "air rho can not be negative");
    }
    if (nu<0.0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "nu can not be negative");
    }
    if (g<0.0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "g can not be negative");
    }
}

void EnvironmentAndFrames::feed(
        Observer& observer, double t,
        const std::vector<BodyPtr>& bodies, const StateType& state) const
{
    try
    {
        if (w.get())
        {
            for (size_t i = 0 ; i < bodies.size() ; ++i)
            {
                bodies[i]->update_kinematics(state,k);
            }
            const auto kk = w->get_waves_on_mesh_as_a_grid(k, t);
            if(kk.z.size()!=0)
            {
                const auto address = DataAddressing(std::vector<std::string>{"waveElevation"},"waves");
                observer.write(kk, address);
            }
        }
    }
    catch (const ssc::kinematics::KinematicsException& e)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException,
                "In the YAML file (section [environment models/model/output/frame of reference]): the output reference frame is not defined (" << e.get_message() << ")");
    }
}

void EnvironmentAndFrames::set_rho_air(double value)
{
	rho_air = value;
}

double EnvironmentAndFrames::get_rho_air() const
{
	if(not(rho_air.is_initialized()))
	{
		THROW(__PRETTY_FUNCTION__, InvalidInputException,"The value of rho_air was requested, but was not specified in the input YAML file. Make sure the key 'air rho' is present in the 'environmental constants' section.")
	}
	return rho_air.get();
}
