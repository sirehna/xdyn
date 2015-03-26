/*
 * HydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "calculate_gz.hpp"
#include "mesh_manipulations.hpp"
#include "Observer.hpp"

const std::string FastHydrostaticForceModel::model_name = "non-linear hydrostatic (fast)";

FastHydrostaticForceModel::FastHydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(model_name, body_name_, env_)
{
}

double FastHydrostaticForceModel::gz() const
{
    return calculate_gz(env.k->get("NED", get_body_name()), get_force_in_ned_frame());
}

SurfaceForceModel::DF FastHydrostaticForceModel::dF(const FacetIterator& that_facet, const EnvironmentAndFrames& env, const BodyStates& states, const double) const
{
    if (that_facet->area == 0) return DF(EPoint(0,0,0),EPoint(0,0,0));
    const double zG = zg_calculator->get_zG_in_NED(that_facet->centre_of_gravity);
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const EPoint C = get_application_point(that_facet, states, zG);
    return DF(-env.rho*env.g*zG*dS,C);
}

EPoint FastHydrostaticForceModel::get_application_point(const FacetIterator& that_facet, const BodyStates&, const double) const
{
    return that_facet->centre_of_gravity; // In Body frame
}

double FastHydrostaticForceModel::pe(const BodyStates& states, const std::vector<double>&, const EnvironmentAndFrames& env) const
{
    const auto G = states.intersector->center_of_mass_immersed();
    const double zC = G.G(2);
    const double immersed_volume = G.volume;
    return -env.rho*env.g*immersed_volume*zC;
}

void FastHydrostaticForceModel::extra_observations(Observer& observer) const
{
    observer.write(gz(),DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),get_body_name(),"GZ"},std::string("GZ(")+get_name()+","+get_body_name()+")"));
}
