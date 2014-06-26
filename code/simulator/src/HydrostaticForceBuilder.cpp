/*
 * HydrostaticForceBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "HydrostaticForceBuilder.hpp"
#include "force_parsers.hpp"
#include "EnvironmentAndFrames.hpp"
#include "HydrostaticForceModel.hpp"

boost::optional<ForcePtr> ForceBuilder<HydrostaticForceModel>::try_to_parse(const std::string& model, const std::string&, const EnvironmentAndFrames& env) const
{
    boost::optional<ForcePtr> ret;
    if (model == "non-linear hydrostatic")
    {
        HydrostaticForceModel::Input input(env);
        ret.reset(ForcePtr(new HydrostaticForceModel(input)));
    }
    return ret;
}

