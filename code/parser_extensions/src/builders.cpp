/*
 * builders.cpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#include "builders.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "environment_parsers.hpp"
#include "SimulatorBuilderException.hpp"
#include "force_parsers.hpp"
#include "EnvironmentAndFrames.hpp"
#include "GravityForceModel.hpp"

TR1(shared_ptr)<SurfaceElevationInterface> build_default_wave_model(const std::string& yaml);
TR1(shared_ptr)<SurfaceElevationInterface> build_default_wave_model(const std::string& yaml)
{
    if (yaml.empty())
    {
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, "No yaml data detected for default wave model (expected 'constant wave height in NED frame: {value: xx, unit: yy})'");
    }
    return TR1(shared_ptr)<SurfaceElevationInterface>(new DefaultSurfaceElevation(parse_default_wave_model(yaml)));
}


boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > SurfaceElevationBuilder<DefaultSurfaceElevation>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > ret;
    if (model == "no waves") ret.reset(build_default_wave_model(yaml));
    return ret;
}

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

boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > SurfaceElevationBuilder<Airy>::try_to_parse(const std::string& model, const std::string& ) const
{
    boost::optional<TR1(shared_ptr)<SurfaceElevationInterface> > ret;
    if (model == "waves") // The "model" key is always "wave", except for the default wave model "no waves"
    {
    }
    return ret;
}
