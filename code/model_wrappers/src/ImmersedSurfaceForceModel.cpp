/*
 * ImmersedSurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "ImmersedSurfaceForceModel.hpp"

ImmersedSurfaceForceModel::ImmersedSurfaceForceModel(const std::string& name_, const EnvironmentAndFrames& env) : SurfaceForceModel(name_, env)
{
}

ImmersedSurfaceForceModel::~ImmersedSurfaceForceModel()
{
}

FacetIterator ImmersedSurfaceForceModel::begin(const MeshIntersectorPtr& intersector) const
{
    return intersector->begin_immersed();
}

FacetIterator ImmersedSurfaceForceModel::end(const MeshIntersectorPtr& intersector) const
{
    return intersector->end_immersed();
}
