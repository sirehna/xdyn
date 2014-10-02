/*
 * EmergedSurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "EmergedSurfaceForceModel.hpp"

EmergedSurfaceForceModel::EmergedSurfaceForceModel(const EnvironmentAndFrames& env) : SurfaceForceModel(env)
{
}

EmergedSurfaceForceModel::~EmergedSurfaceForceModel()
{
}

FacetIterator EmergedSurfaceForceModel::begin(const MeshIntersectorPtr& intersector) const
{
    return intersector->begin_emerged();
}

FacetIterator EmergedSurfaceForceModel::end(const MeshIntersectorPtr& intersector) const
{
    return intersector->end_emerged();
}
