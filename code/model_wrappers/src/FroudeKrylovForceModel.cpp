/*
 * FroudeKrylovForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "SurfaceElevationInterface.hpp"

FroudeKrylovForceModel::FroudeKrylovForceModel(const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(env_)
{
}

SurfaceForceModel::DF FroudeKrylovForceModel::dF(const FacetIterator& that_facet, const EnvironmentAndFrames& env, const Body& body, const double t) const
{
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const ssc::kinematics::Point C(body.M->get_frame(), that_facet->barycenter);//get_application_point(that_facet, body, zG);
    const double pdyn = env.w->get_dynamic_pressure(env.rho,env.g,C,env.k,t);
    return DF(-pdyn*dS,C.v);
}

double FroudeKrylovForceModel::pe(const Body& , const std::vector<double>& , const EnvironmentAndFrames& ) const
{
    return 0;
}
