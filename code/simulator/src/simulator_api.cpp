#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"

#include "GravityForceModel.hpp"
#include "DefaultWaveModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "StlReader.hpp"
#include "SimulatorBuilder.hpp"

SimulatorBuilder get_builder(const std::string& yaml)
{
    SimulatorBuilder builder(SimulatorYamlParser(yaml).parse());
    builder.can_parse<GravityForceModel>()
           .can_parse<DefaultWaveModel>()
           .can_parse<HydrostaticForceModel>();
    return builder;
}

Sim get_system(const std::string& yaml)
{
    return get_builder(yaml).build();
}

Sim get_system(const std::string& yaml, const std::string& mesh)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return get_system(yaml, meshes);
}

Sim get_system(const std::string& yaml, const MeshMap& meshes)
{
    return get_builder(yaml).build(meshes);
}
