#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"

#include "Airy.hpp"
#include "BretschneiderSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "GravityForceModel.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "JonswapSpectrum.hpp"
#include "PiersonMoskowitzSpectrum.hpp"
#include "StlReader.hpp"
#include "SimulatorBuilder.hpp"
#include "SurfaceElevationFromWaves.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "QuadraticDampingForceModel.hpp"
#include "LinearDampingForceModel.hpp"

SimulatorBuilder get_builder(const YamlSimulatorInput& yaml)
{
    SimulatorBuilder builder(yaml);
    builder.can_parse<GravityForceModel>()
           .can_parse<DefaultSurfaceElevation>()
           .can_parse<ExactHydrostaticForceModel>()
           .can_parse<FastHydrostaticForceModel>()
           .can_parse<BretschneiderSpectrum>()
           .can_parse<JonswapSpectrum>()
           .can_parse<PiersonMoskowitzSpectrum>()
           .can_parse<DiracSpectralDensity>()
           .can_parse<DiracDirectionalSpreading>()
           .can_parse<Cos2sDirectionalSpreading>()
           .can_parse<SurfaceElevationFromWaves>()
           .can_parse<Airy>()
           .can_parse<FroudeKrylovForceModel>()
           .can_parse<QuadraticDampingForceModel>()
           .can_parse<LinearDampingForceModel>();
    return builder;
}

Sim get_system(const YamlSimulatorInput& yaml)
{
    return get_builder(yaml).build();
}

Sim get_system(const YamlSimulatorInput& input, const std::string& mesh)
{
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return get_system(input, meshes);
}

Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    const auto name = input.bodies.empty() ? "" : input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = mesh;
    return get_system(input, meshes);
}

Sim get_system(const YamlSimulatorInput& yaml, const MeshMap& meshes)
{
    return get_builder(yaml).build(meshes);
}

Sim get_system(const std::string& yaml)
{
    return get_system(SimulatorYamlParser(yaml).parse());
}

Sim get_system(const std::string& yaml, const std::string& mesh)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(check_input_yaml(input), mesh);
}

Sim get_system(const std::string& yaml, const MeshMap& meshes)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(check_input_yaml(input), meshes);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh)
{
    const auto name = yaml.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return meshes;
}
