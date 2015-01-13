/*
 * EmergedSurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "EmergedSurfaceForceModel.hpp"

EmergedSurfaceForceModel::EmergedSurfaceForceModel(const std::string& name_, const std::string& body_name_, const EnvironmentAndFrames& env) : SurfaceForceModel(name_, body_name_, env)
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
