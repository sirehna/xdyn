#include "SimulatorYamlParser.hpp"
#include "DataSourceBuilder.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"
#include "DsSystemMacros.hpp"

#include "GravityForceModel.hpp"
#include "DefaultWaveModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "StlReader.hpp"
#include "SimulatorBuilder.hpp"

void set_data_source_solver(DataSource& ds,const std::string& solver);
void set_data_source_solver(DataSource& ds,const std::string& solver)
{
    if (solver=="euler")
    {
        SET(ds, simulator_base::stepper, solver::EULER);
    }
    else if (solver=="rk4")
    {
        SET(ds, simulator_base::stepper, solver::RK4);
    }
    else if (solver=="rkck")
    {
        SET(ds, simulator_base::stepper, solver::RKCK54);
    }
    else
    {
        SET(ds, simulator_base::stepper, solver::EULER);
    }
}

DataSource build(DataSourceBuilder& builder, const double dt, const std::string& solver_type);

DataSource build(DataSourceBuilder& builder, const double dt, const std::string& solver_type)
{
    DataSource ds = builder.build_ds();
    ds.check_in(__PRETTY_FUNCTION__);
    ds.set<double>("simulator_base_initial_time_step",dt);
    set_data_source_solver(ds, solver_type);
    ds.check_out();
    return ds;
}


DataSource make_ds(const std::string& data, const double dt, const std::string& solver_type)
{
    DataSourceBuilder builder(check_input_yaml(SimulatorYamlParser(data).parse()));
    return build(builder, dt, solver_type);
}

DataSource make_ds(const std::string& data,
                   const std::map<std::string, VectorOfVectorOfPoints>& input_meshes,
                   const double dt,
                   const std::string& solver_type)
{
    DataSourceBuilder builder(check_input_yaml(SimulatorYamlParser(data).parse()), input_meshes);
    return build(builder, dt, solver_type);
}

SimulatorBuilder get_builder(const std::string& yaml);
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
