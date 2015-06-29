#include "SimulatorYamlParser.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"

#include "Airy.hpp"
#include "BretschneiderSpectrum.hpp"
#include "Cos2sDirectionalSpreading.hpp"
#include "DiffractionForceModel.hpp"
#include "DiracDirectionalSpreading.hpp"
#include "DiracSpectralDensity.hpp"
#include "GravityForceModel.hpp"
#include "DefaultSurfaceElevation.hpp"
#include "ExactHydrostaticForceModel.hpp"
#include "FastHydrostaticForceModel.hpp"
#include "JonswapSpectrum.hpp"
#include "PiersonMoskowitzSpectrum.hpp"
#include "stl_reader.hpp"
#include "SimulatorBuilder.hpp"
#include "SurfaceElevationFromWaves.hpp"
#include "FroudeKrylovForceModel.hpp"
#include "QuadraticDampingForceModel.hpp"
#include "LinearDampingForceModel.hpp"
#include "WageningenControlledForceModel.hpp"
#include "ResistanceCurveForceModel.hpp"
#include "RadiationDampingForceModel.hpp"
#include "SimpleHeadingKeepingController.hpp"
#include "ManeuveringForceModel.hpp"
#include "SimpleStationKeepingController.hpp"
#include "RudderForceModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "GMForceModel.hpp"
#include "KtKqForceModel.hpp"

SimulatorBuilder get_builder(const YamlSimulatorInput& yaml, const double t0, const ssc::data_source::DataSource& command_listener)
{
    SimulatorBuilder builder(yaml, t0, command_listener);
    builder.can_parse<DefaultSurfaceElevation>()
           .can_parse<BretschneiderSpectrum>()
           .can_parse<JonswapSpectrum>()
           .can_parse<PiersonMoskowitzSpectrum>()
           .can_parse<DiracSpectralDensity>()
           .can_parse<DiracDirectionalSpreading>()
           .can_parse<Cos2sDirectionalSpreading>()
           .can_parse<SurfaceElevationFromWaves>()
           .can_parse<Airy>()
           .can_parse<WageningenControlledForceModel>()
           .can_parse<GravityForceModel>()
           .can_parse<ExactHydrostaticForceModel>()
           .can_parse<FastHydrostaticForceModel>()
           .can_parse<FroudeKrylovForceModel>()
           .can_parse<LinearDampingForceModel>()
           .can_parse<ResistanceCurveForceModel>()
           .can_parse<DiffractionForceModel>()
           .can_parse<RadiationDampingForceModel>()
           .can_parse<QuadraticDampingForceModel>()
           .can_parse<SimpleHeadingKeepingController>()
           .can_parse<ManeuveringForceModel>()
           .can_parse<SimpleStationKeepingController>()
           .can_parse<RudderForceModel>()
           .can_parse<HydrostaticForceModel>()
           .can_parse<GMForceModel>()
           .can_parse<KtKqForceModel>();
    return builder;
}

Sim get_system(const YamlSimulatorInput& yaml, const double t0, const ssc::data_source::DataSource& command_listener)
{
    return get_builder(yaml, t0, command_listener).build();
}

Sim get_system(const YamlSimulatorInput& input, const std::string& mesh, const double t0, const ssc::data_source::DataSource& command_listener)
{
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return get_system(input, meshes, t0, command_listener);
}

Sim get_system(const YamlSimulatorInput& input, const VectorOfVectorOfPoints& mesh, const double t0, const ssc::data_source::DataSource& command_listener)
{
    const auto name = input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = mesh;
    return get_system(input, meshes, t0, command_listener);
}

Sim get_system(const std::string& yaml, const VectorOfVectorOfPoints& mesh, const double t0, const ssc::data_source::DataSource& command_listener)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    const auto name = input.bodies.empty() ? "" : input.bodies.front().name;
    MeshMap meshes;
    meshes[name] = mesh;
    return get_system(input, meshes, t0, command_listener);
}

Sim get_system(const YamlSimulatorInput& yaml, const MeshMap& meshes, const double t0, const ssc::data_source::DataSource& command_listener)
{
    return get_builder(yaml, t0, command_listener).build(meshes);
}

Sim get_system(const std::string& yaml, const double t0, const ssc::data_source::DataSource& command_listener)
{
    return get_system(SimulatorYamlParser(yaml).parse(), t0, command_listener);
}

Sim get_system(const std::string& yaml, const std::string& mesh, const double t0, const ssc::data_source::DataSource& command_listener)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(check_input_yaml(input), mesh, t0, command_listener);
}

Sim get_system(const std::string& yaml, const MeshMap& meshes, const double t0, const ssc::data_source::DataSource& command_listener)
{
    const auto input = SimulatorYamlParser(yaml).parse();
    return get_system(check_input_yaml(input), meshes, t0, command_listener);
}

MeshMap make_mesh_map(const YamlSimulatorInput& yaml, const std::string& mesh)
{
    const auto name = yaml.bodies.front().name;
    MeshMap meshes;
    meshes[name] = read_stl(mesh);
    return meshes;
}
