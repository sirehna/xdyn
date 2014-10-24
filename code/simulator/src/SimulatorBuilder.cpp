/*
 * SimulatorBuilder.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include <ssc/kinematics.hpp>
#include <ssc/text_file_reader.hpp>
#include "SimulatorBuilder.hpp"
#include "SimulatorBuilderException.hpp"
#include "update_kinematics.hpp"
#include "StlReader.hpp"
#include "BodyBuilder.hpp"

SimulatorBuilder::SimulatorBuilder(const YamlSimulatorInput& input_, const ssc::data_source::DataSource& command_listener_) :
                                        input(input_),
                                        builder(TR1(shared_ptr)<BodyBuilder>(new  BodyBuilder(input.rotations))),
                                        force_parsers(std::vector<ForceBuilderPtr>()),
                                        controlled_force_parsers(std::vector<ControlledForceBuilderPtr>()),
                                        surface_elevation_parsers(std::vector<SurfaceElevationBuilderPtr>()),
                                        wave_parsers(TR1(shared_ptr)<std::vector<WaveModelBuilderPtr> >(new std::vector<WaveModelBuilderPtr>())),
                                        directional_spreading_parsers(TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >(new std::vector<DirectionalSpreadingBuilderPtr>())),
                                        spectrum_parsers(TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >(new std::vector<SpectrumBuilderPtr>())),
                                        command_listener(command_listener_)
{
}

std::vector<Body> SimulatorBuilder::get_bodies(const MeshMap& meshes) const
{
    std::vector<Body> ret;
    for (auto that_body=input.bodies.begin() ; that_body != input.bodies.end() ; ++that_body)
    {
        const auto that_mesh = meshes.find(that_body->name);
        if (that_mesh != meshes.end())
        {
            ret.push_back(builder->build(*that_body, that_mesh->second));
        }
        else
        {
            THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, std::string("Unable to find mesh for '") + that_body->name + "' in map");
        }
    }
    return ret;
}

EnvironmentAndFrames SimulatorBuilder::get_environment_and_frames(const std::vector<Body>& bodies) const
{
    EnvironmentAndFrames env;
    env.g = input.environmental_constants.g;
    env.rho = input.environmental_constants.rho;
    env.rot = input.rotations;
    env.k = KinematicsPtr(new ssc::kinematics::Kinematics());
    if (bodies.size() != input.bodies.size())
    {
        std::stringstream ss;
        ss << "YAML data contains " << input.bodies.size() << " bod";
        if (input.bodies.size()>1) ss << "ies";
        else                       ss << "y";
        ss << ", but method received " << bodies.size() << " Body object";
        if (bodies.size()>1) ss << "s";
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, ss.str());
    }
    const StateType x = ::get_initial_states(input.rotations, input.bodies);
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        env.k->add(get_transform_from_mesh_to(bodies.at(i)));
        env.k->add(get_transform_from_ned_to(x, bodies.at(i), i));
    }
    env.w = get_wave();
    return env;
}

SurfaceElevationPtr SimulatorBuilder::get_wave() const
{
    if (surface_elevation_parsers.empty())
    {
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, "No wave parser defined: use SimulatorBuilder::can_parse<T> with e.g. T=DefaultWaveModel");
    }
    SurfaceElevationPtr ret;
    for (auto that_model=input.environment.begin() ; that_model != input.environment.end() ; ++that_model)
    {
        for (auto that_parser=surface_elevation_parsers.begin() ; that_parser != surface_elevation_parsers.end() ; ++that_parser)
        {
            boost::optional<SurfaceElevationPtr> w = (*that_parser)->try_to_parse(that_model->model, that_model->yaml);
            if (w)
            {
                if (ret.use_count())
                {
                    THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, "More than one wave model was defined.");
                }
                ret = w.get();
            }
        }
    }
    return ret;
}

std::vector<ListOfForces> SimulatorBuilder::get_forces(const EnvironmentAndFrames& env) const
{
    std::vector<ListOfForces> forces;
    for (auto that_body=input.bodies.begin() ; that_body != input.bodies.end() ; ++that_body)
    {
        forces.push_back(forces_from(*that_body, env));
    }
    return forces;
}

std::vector<ListOfControlledForces> SimulatorBuilder::get_controlled_forces(const EnvironmentAndFrames& env) const
{
    std::vector<ListOfControlledForces> forces;
    for (auto that_body=input.bodies.begin() ; that_body != input.bodies.end() ; ++that_body)
    {
        forces.push_back(controlled_forces_from(*that_body, env));
    }
    return forces;
}

ListOfForces SimulatorBuilder::forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const
{
    ListOfForces ret;
    for (auto that_force_model = body.external_forces.begin() ; that_force_model!= body.external_forces.end() ; ++that_force_model)
    {
        add(*that_force_model, ret, env);
    }
    return ret;
}

ListOfControlledForces SimulatorBuilder::controlled_forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const
{
    ListOfControlledForces ret;
    for (auto that_force_model = body.controlled_forces.begin() ; that_force_model!= body.controlled_forces.end() ; ++that_force_model)
    {
        add(*that_force_model, ret, env);
    }
    return ret;
}

void SimulatorBuilder::add(const YamlModel& model, ListOfForces& L, const EnvironmentAndFrames& env) const
{
    bool parsed = false;
    for (auto that_parser = force_parsers.begin() ; that_parser != force_parsers.end() ; ++that_parser)
    {
        boost::optional<ForcePtr> f = (*that_parser)->try_to_parse(model.model, model.yaml, env);
        if (f)
        {
            L.push_back(f.get());
            parsed = true;
        }
    }
    if (not(parsed))
    {
        std::stringstream ss;
        ss << "Unable to find a parser to parse model '" << model.model << "'";
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, ss.str());
    }
}

void SimulatorBuilder::add(const YamlModel& model, ListOfControlledForces& L, const EnvironmentAndFrames& env) const
{
    bool parsed = false;
    for (auto that_parser = controlled_force_parsers.begin() ; that_parser != controlled_force_parsers.end() ; ++that_parser)
    {
        boost::optional<ControlledForcePtr> f = (*that_parser)->try_to_parse(model.model, model.yaml, env);
        if (f)
        {
            L.push_back(f.get());
            parsed = true;
        }
    }
    if (not(parsed))
    {
        std::stringstream ss;
        ss << "Unable to find a parser to parse model '" << model.model << "'";
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, ss.str());
    }
}

Sim SimulatorBuilder::build(const MeshMap& meshes) const
{
    const auto bodies = get_bodies(meshes);
    const auto env = get_environment_and_frames(bodies);
    return Sim(bodies, get_forces(env), get_controlled_forces(env), env, get_initial_states(), command_listener);
}

StateType SimulatorBuilder::get_initial_states() const
{
    return ::get_initial_states(input.rotations, input.bodies);
}

Sim SimulatorBuilder::build() const
{
    return build(make_mesh_map());
}

YamlSimulatorInput SimulatorBuilder::get_parsed_yaml() const
{
    return input;
}

MeshMap SimulatorBuilder::make_mesh_map() const
{
    MeshMap ret;
    for (auto that_body = input.bodies.begin() ; that_body != input.bodies.end() ; ++that_body)
    {
        ret[that_body->name] = get_mesh(*that_body);
    }
    return ret;
}

VectorOfVectorOfPoints SimulatorBuilder::get_mesh(const YamlBody& body) const
{
    if (not(body.mesh.empty()))
    {
        const ssc::text_file_reader::TextFileReader reader(std::vector<std::string>(1, body.mesh));
        return read_stl(reader.get_contents());
    }
    return VectorOfVectorOfPoints();
}
