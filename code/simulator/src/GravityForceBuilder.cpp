/*
 * GravityForceBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "GravityForceBuilder.hpp"
#include "force_parsers.hpp"
#include "EnvironmentAndFrames.hpp"
#include "GravityForceModel.hpp"

boost::optional<ForcePtr> ForceBuilder<GravityForceModel>::try_to_parse(const std::string& model, const std::string& , const EnvironmentAndFrames& env) const
{
    boost::optional<ForcePtr> ret;
    if (model == "gravity")
    {
        GravityForceModel::Input input(env);
        ret.reset(ForcePtr(new GravityForceModel(input)));
    }
    return ret;
}

