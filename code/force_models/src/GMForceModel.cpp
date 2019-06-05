/*
 * GMForceModel.cpp
 *
 *  Created on: Apr 13, 2015
 *      Author: cady
 */

#include "calculate_gz.hpp"
#include "GMForceModel.hpp"
#include "yaml.h"
#include <ssc/yaml_parser.hpp>
#include "environment_parsers.hpp"
#include "Body.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "Observer.hpp"
#include "InvalidInputException.hpp"
#include "BodyWithSurfaceForces.hpp"

std::string GMForceModel::model_name(){return "GM";}

GMForceModel::Yaml::Yaml() : name_of_hydrostatic_force_model(), roll_step(0)
{
}

GMForceModel::GMForceModel(const Yaml& data, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ImmersedSurfaceForceModel(model_name(), body_name_, env_)
, underlying_hs_force_model()
, dphi(data.roll_step)
, env(env_)
, GM(new double(0))
, GZ(new double(0))
{
    if (env.w.use_count()==0)
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Force model '" << model_name << "' needs a wave model, even if it's 'no waves'");
    }
    YamlModel data_for_hs;
    data_for_hs.index_of_first_line_in_global_yaml = data.index_of_first_line_in_global_yaml;
    data_for_hs.model = data.name_of_hydrostatic_force_model;
    boost::optional<ForcePtr> f = data.try_to_parse(data_for_hs, get_body_name(), env);
    if (f)
    {
        underlying_hs_force_model = f.get();
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Unable to find a parser to parse model '" << data.name_of_hydrostatic_force_model << "'");
    }
}

double GMForceModel::get_GM() const
{
    return *this->GM;
}

GMForceModel::Yaml GMForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret;
    node["name of hydrostatic force model"] >> ret.name_of_hydrostatic_force_model;
    ssc::yaml_parser::parse_uv(node["roll step"], ret.roll_step);
    if (ret.name_of_hydrostatic_force_model == "hydrostatic")
    {
        ret.try_to_parse = ForceModel::build_parser<HydrostaticForceModel>();
        return ret;
    }
    if (ret.name_of_hydrostatic_force_model == "non-linear hydrostatic (exact)")
    {
        ret.try_to_parse = ForceModel::build_parser<ExactHydrostaticForceModel>();
        return ret;
    }
    if (ret.name_of_hydrostatic_force_model == "non-linear hydrostatic (fast)")
    {
        ret.try_to_parse = ForceModel::build_parser<FastHydrostaticForceModel>();
        return ret;
    }
    THROW(__PRETTY_FUNCTION__, InvalidInputException, "Couldn't find any suitable hydrostatic force model: "
            << "received '" << ret.name_of_hydrostatic_force_model << "', expected one of 'non-linear hydrostatic (exact)', 'non-linear hydrostatic (fast)' or 'hydrostatic'");
    return ret;
}

BodyStates GMForceModel::get_shifted_states(const BodyStates& states,
        const double t) const
{
    auto euler = states.get_angles(states.convention);
    euler.phi -= dphi;
    const auto quaternions = states.convert(euler, states.convention);
    BodyStates new_states = states;
    new_states.qr.record(t + 1, std::get<0>(quaternions));
    new_states.qi.record(t + 1, std::get<1>(quaternions));
    new_states.qj.record(t + 1, std::get<2>(quaternions));
    new_states.qk.record(t + 1, std::get<3>(quaternions));
    return new_states;
}

double GMForceModel::get_gz_for_shifted_states(const BodyStates& states, const double t) const
{
    BodyStates new_states = get_shifted_states(states, t);
    BodyWithSurfaceForces body_for_gm(new_states, 0, BlockedDOF(""));
    body_for_gm.reset_history();
    body_for_gm.update(env, new_states.get_current_state_values(0), t);
    underlying_hs_force_model->update(body_for_gm.get_states(), t);
    return calculate_gz(*underlying_hs_force_model, env);
}

ssc::kinematics::Wrench GMForceModel::operator()(const BodyStates& states, const double t) const
{
    underlying_hs_force_model->update(states, t);
    const auto ret = underlying_hs_force_model->get_force_in_body_frame();
    const double gz1 = calculate_gz(*underlying_hs_force_model, env);
    const double gz2 = get_gz_for_shifted_states(states, t);
    *GM = (gz1-gz2)/dphi;
    *GZ = (gz1+gz2)/2;
    return ret;
}

void GMForceModel::extra_observations(Observer& observer) const
{
    observer.write(*GM,DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"GM"},std::string("GM(") + get_body_name() + ")"));
    observer.write(*GZ,DataAddressing(std::vector<std::string>{"efforts",get_body_name(),get_name(),"GM"},std::string("GZ(") + get_body_name() + ")"));
}

double GMForceModel::pe(const BodyStates& , const std::vector<double>& , const EnvironmentAndFrames& ) const
{
    return 0;
}

std::function<GMForceModel::DF(const FacetIterator &, const size_t, const EnvironmentAndFrames &, const BodyStates &, const double)>
    GMForceModel::get_dF(const FacetIterator& begin_facet,
                         const FacetIterator& end_facet,
                         const EnvironmentAndFrames& env,
                         const BodyStates& states,
                         const double t) const
{
    return [](const FacetIterator &,
              const size_t,
              const EnvironmentAndFrames &,
              const BodyStates &,
              const double t)
    {
        return GMForceModel::DF(EPoint(), EPoint());
    };
}
