/*
 * HydrostaticForceModel.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "HydrostaticForceModel.hpp"
#include "hydrostatic.hpp"

FastHydrostaticForceModel::FastHydrostaticForceModel(const EnvironmentAndFrames& env_) : ImmersedSurfaceForceModel(env_)
{
}

SurfaceForceModel::DF FastHydrostaticForceModel::dF(const FacetIterator& that_facet, const MeshIntersectorPtr& intersector, const EnvironmentAndFrames& env, const Body&, const double) const
{
    const double zG = hydrostatic::average_immersion(that_facet->vertex_index, intersector->all_immersions);
    const EPoint dS = that_facet->area*that_facet->unit_normal;
    const EPoint C = that_facet->barycenter;
    return DF(-env.rho*env.g*zG*dS,C);
}

