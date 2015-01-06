/*
 * ExactHydrostaticForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "mesh_manipulations.hpp"

const std::string ExactHydrostaticForceModel::model_name = "exact hydrostatic";

ExactHydrostaticForceModel::ExactHydrostaticForceModel(const EnvironmentAndFrames& env_) : FastHydrostaticForceModel(env_)
{
}

EPoint ExactHydrostaticForceModel::get_application_point(const FacetIterator& that_facet,
                                                         const Body& body,
                                                         const double zG) const
{
    return exact_application_point(that_facet,body.down_direction_in_mesh_frame,zG,body.intersector->mesh->all_nodes,body.intersector->all_relative_immersions);
}
