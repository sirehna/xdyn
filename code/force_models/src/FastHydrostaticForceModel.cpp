/*
 * HydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "mesh_manipulations.hpp"

const std::string FastHydrostaticForceModel::model_name = "non-linear hydrostatic (fast)";

FastHydrostaticForceModel::FastHydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(model_name, body_name_, env_)
{
}

SurfaceForceModel::DF FastHydrostaticForceModel::dF(const FacetIterator& that_facet, const EnvironmentAndFrames& env, const BodyStates& states, const double) const
{
    if (that_facet->area == 0) return DF(EPoint(0,0,0),EPoint(0,0,0));
    const double zG = zg_calculator->get_zG_in_NED(that_facet->barycenter);
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const EPoint C = get_application_point(that_facet, states, zG);
    return DF(-env.rho*env.g*zG*dS,C);
}

EPoint FastHydrostaticForceModel::get_application_point(const FacetIterator& that_facet, const BodyStates&, const double) const
{
    return that_facet->barycenter; // In Body frame
}

double FastHydrostaticForceModel::pe(const BodyStates& states, const std::vector<double>&, const EnvironmentAndFrames& env) const
{
    const auto b = states.intersector->begin_immersed();
    const auto e = states.intersector->end_immersed();
    const auto G = states.intersector->center_of_mass(b, e);
    const double zC = G.G(2);
    const double Vim = states.intersector->immersed_volume();
    return -env.rho*env.g*Vim*zC;
}

