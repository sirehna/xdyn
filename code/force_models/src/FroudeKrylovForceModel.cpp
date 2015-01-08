/*
 * FroudeKrylovForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "SurfaceElevationInterface.hpp"

const std::string FroudeKrylovForceModel::model_name = "non-linear Froude-Krylov";

FroudeKrylovForceModel::FroudeKrylovForceModel(const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel("froude-krylov", env_)
{
}

SurfaceForceModel::DF FroudeKrylovForceModel::dF(const FacetIterator& that_facet, const EnvironmentAndFrames& env, const BodyStates& states, const double t) const
{
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const ssc::kinematics::Point C(states.M->get_frame(), that_facet->barycenter);
    double eta = 0;
    for (auto it = that_facet->vertex_index.begin() ; it != that_facet->vertex_index.end() ; ++it)
    {
        eta += states.intersector->all_absolute_wave_elevations.at(*it);
    }
    if (not(that_facet->vertex_index.empty())) eta /= (double)that_facet->vertex_index.size();
    const double pdyn = env.w->get_dynamic_pressure(env.rho,env.g,C,env.k,eta,t);
    return DF(pdyn*dS,C.v);
}

double FroudeKrylovForceModel::pe(const BodyStates& , const std::vector<double>& , const EnvironmentAndFrames& ) const
{
    return 0;
}
