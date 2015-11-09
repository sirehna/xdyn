/*
 * ExactHydrostaticForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "mesh_manipulations.hpp"

const std::string ExactHydrostaticForceModel::model_name = "non-linear hydrostatic (exact)";

ExactHydrostaticForceModel::ExactHydrostaticForceModel(const std::string& body_name_, const EnvironmentAndFrames& env_) : FastHydrostaticForceModel(ExactHydrostaticForceModel::model_name, body_name_, env_)
{
    if (env.w.use_count()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << model_name << "' needs a wave model, even if it's 'no waves'");
    }
}

EPoint ExactHydrostaticForceModel::get_application_point(const FacetIterator& that_facet,
                                                         const BodyStates& states,
                                                         const double zG) const
{
    return exact_application_point(that_facet,states.g_in_mesh_frame,zG,states.intersector->mesh->all_nodes,states.intersector->all_relative_immersions);
}
