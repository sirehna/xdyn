/*
 * SimulatorBuilder.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "SimulatorBuilder.hpp"

#include "InternalErrorException.hpp"
#include "update_kinematics.hpp"
#include "stl_reader.hpp"
#include "BodyBuilder.hpp"

#include <ssc/text_file_reader.hpp>

SimulatorBuilder::SimulatorBuilder(const YamlSimulatorInput& input_, const double t0_, const ssc::data_source::DataSource& command_listener_) :
                                        input(input_),
                                        builder(TR1(shared_ptr)<BodyBuilder>(new  BodyBuilder(input.rotations))),
                                        force_parsers(),
                                        controllable_force_parsers(),
                                        surface_elevation_parsers(),
                                        wave_parsers(TR1(shared_ptr)<std::vector<WaveModelBuilderPtr> >(new std::vector<WaveModelBuilderPtr>())),
                                        directional_spreading_parsers(TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >(new std::vector<DirectionalSpreadingBuilderPtr>())),
                                        spectrum_parsers(TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >(new std::vector<SpectrumBuilderPtr>())),
										wind_model_parsers(),
                                        command_listener(command_listener_),
                                        t0(t0_)
{
}

std::vector<BodyPtr> SimulatorBuilder::get_bodies(const MeshMap& meshes, const std::vector<bool>& bodies_contain_surface_forces, std::map<std::string,double> history_length) const
{
    std::vector<BodyPtr> ret;
    size_t i = 0;
    for (const auto body:input.bodies)
    {
        const auto that_mesh = meshes.find(body.name);
        if (that_mesh != meshes.end())
        {
            ret.push_back(builder->build(body, that_mesh->second, i,t0,input.rotations,history_length[body.name],bodies_contain_surface_forces.at(i)));
            i++;
        }
        else
        {
            THROW(__PRETTY_FUNCTION__, InternalErrorException, "Unable to find mesh for '" << body.name << "' in map.");
        }
    }
    return ret;
}

void SimulatorBuilder::add_initial_transforms(const std::vector<BodyPtr>& bodies, ssc::kinematics::KinematicsPtr& k) const
{
    const StateType x = ::get_initial_states(input.rotations, input.bodies);
    for (size_t i = 0; i < bodies.size(); ++i)
    {
        k->add(bodies.at(i)->get_transform_from_mesh_to_body());
        k->add(bodies.at(i)->get_transform_from_ned_to_body(x));
    }
}

EnvironmentAndFrames SimulatorBuilder::get_environment() const
{
    EnvironmentAndFrames env;
    env.g = input.environmental_constants.g;
    env.rho = input.environmental_constants.rho;
    env.nu = input.environmental_constants.nu;
    env.rot = input.rotations;
    env.w = get_wave();
    env.k = ssc::kinematics::KinematicsPtr(new ssc::kinematics::Kinematics());
    return env;
}

SurfaceElevationPtr SimulatorBuilder::get_wave() const
{
    if (surface_elevation_parsers.empty())
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "No wave parser defined. Need to call SimulatorBuilder::can_parse<T> with e.g. T=DefaultWaveModel");
    }
    SurfaceElevationPtr ret;
    for (auto that_model=input.environment.begin() ; that_model != input.environment.end() ; ++that_model)
    {
        bool wave_model_successfully_parsed = false;
        for (auto that_parser=surface_elevation_parsers.begin() ; that_parser != surface_elevation_parsers.end() ; ++that_parser)
        {
            boost::optional<SurfaceElevationPtr> w = (*that_parser)->try_to_parse(that_model->model, that_model->yaml);
            if (w)
            {
                if (ret.use_count())
                {
                    THROW(__PRETTY_FUNCTION__, InternalErrorException, "More than one wave model was defined.");
                }
                ret = w.get();
                wave_model_successfully_parsed = true;
            }
        }
        if (not(wave_model_successfully_parsed))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Simulator does not understand wave model '" << that_model->model << "'");
        }
    }
    return ret;
}

std::vector<ListOfForces> SimulatorBuilder::get_forces(const EnvironmentAndFrames& env) const
{
    std::vector<ListOfForces> forces;
    for (const auto body:input.bodies)
    {
        forces.push_back(forces_from(body, env));
    }
    return forces;
}

std::map<std::string, double> SimulatorBuilder::get_max_history_length(const std::vector<ListOfForces>& forces_for_all_bodies, const std::vector<ListOfControlledForces>& controlled_forces_for_all_bodies) const
{
    std::map<std::string, double> ret;
    for (const auto forces:forces_for_all_bodies)
    {
        double Tmax = 0;
        for (const auto force:forces) Tmax = std::max(Tmax, force->get_Tmax());
        if (not(forces.empty())) ret[forces.front()->get_body_name()] = Tmax;
    }
    for (const auto forces:controlled_forces_for_all_bodies)
    {
        double Tmax = 0;
        for (const auto force:forces) Tmax = std::max(Tmax, force->get_Tmax());
        if (not(forces.empty())) ret[forces.front()->get_body_name()] = std::max(Tmax, ret[forces.front()->get_body_name()]);
    }
    return ret;
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
        add(*that_force_model, ret, body.name, env);
    }
    return ret;
}

ListOfControlledForces SimulatorBuilder::controlled_forces_from(const YamlBody& body, const EnvironmentAndFrames& env) const
{
    ListOfControlledForces ret;
    for (auto that_force_model = body.controlled_forces.begin() ; that_force_model!= body.controlled_forces.end() ; ++that_force_model)
    {
        add(*that_force_model, ret, body.name, env);
    }
    return ret;
}

template <typename T> bool could_parse(const std::vector<T>& parsers, const YamlModel& model, const std::string& body_name, const EnvironmentAndFrames& env)
{
    try
    {
        for (auto try_to_parse:parsers)
        {
            auto f = try_to_parse(model, body_name, env);
            if (f)
            {
                return true;
            }
        }
    }
    catch (const InvalidInputException&)
    {
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void SimulatorBuilder::add(const YamlModel& model, ListOfForces& L, const std::string& body_name, const EnvironmentAndFrames& env) const
{
    bool parsed = false;
    for (auto try_to_parse:force_parsers)
    {
        boost::optional<ForcePtr> f = try_to_parse(model, body_name, env);
        if (f)
        {
            L.push_back(f.get());
            parsed = true;
        }
    }

    if (not(parsed))
    {
        if (could_parse(controllable_force_parsers, model, body_name, env))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Model '" << model.model << "' is in the wrong section: it's in the 'external forces' section whereas it should be in the 'controlled forces' section.");
        }
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Simulator does not know model '" << model.model << "': maybe the name is misspelt or you are using an outdated version of this simulator, or maybe you put a controlled force (eg. maneuvering, propeller+rudder, etc.) in the 'external forces' section.");
    }
}

void SimulatorBuilder::add(const YamlModel& model, ListOfControlledForces& L, const std::string& name, const EnvironmentAndFrames& env) const
{
    bool parsed = false;
    for (auto try_to_parse:controllable_force_parsers)
    {
        boost::optional<ControllableForcePtr> f = try_to_parse(model, name, env);
        if (f)
        {
            L.push_back(f.get());
            parsed = true;
        }
    }
    if (not(parsed))
    {
        if (could_parse(force_parsers, model, name, env))
        {
            THROW(__PRETTY_FUNCTION__, InvalidInputException, "Model '" << model.model << "' is in the wrong section: it's in the 'controlled forces' section whereas it should be in the 'external forces' section.");
        }
        THROW(__PRETTY_FUNCTION__, InvalidInputException, "Simulator does not know model '" << model.model << "': maybe the name is misspelt or you are using an outdated version of this simulator.");
    }
}

std::vector<bool> SimulatorBuilder::are_there_surface_forces_acting_on_body(const std::vector<ListOfForces>& forces) const
{
    std::vector<bool> ret;
    for (auto forces_acting_on_body:forces)
    {
        bool has_surface_forces = false;
        for (auto force:forces_acting_on_body)
        {
            has_surface_forces |= force->is_a_surface_force_model();
        }
        ret.push_back(has_surface_forces);
    }
    return ret;
}

Sim SimulatorBuilder::build(const MeshMap& meshes) const
{
    auto env = get_environment();
    const auto forces = get_forces(env);
    const auto controlled_forces = get_controlled_forces(env);
    auto history_length = get_max_history_length(forces, controlled_forces);
    const auto bodies = get_bodies(meshes, are_there_surface_forces_acting_on_body(forces), history_length);
    add_initial_transforms(bodies, env.k);
    return Sim(bodies, forces, get_controlled_forces(env), env, get_initial_states(), command_listener);
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
        const ssc::text_file_reader::TextFileReader reader(body.mesh);
        return read_stl(reader.get_contents());
    }
    return VectorOfVectorOfPoints();
}
