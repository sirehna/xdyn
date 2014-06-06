#include "SimulatorYamlParser.hpp"
#include "DataSourceBuilder.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"
#include "DsSystemMacros.hpp"

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

DataSource make_ds(const std::string& data, const double dt, const std::string& solver_type)
{
    SimulatorYamlParser parser(data);
    DataSourceBuilder builder(check_input_yaml(parser.parse()));
    DataSource ds = builder.build_ds();
    ds.set<double>("simulator_base_initial_time_step",dt);
    set_data_source_solver(ds, solver_type);
    return ds;
}

DataSource make_ds(const std::string& data,
                   const std::map<std::string, VectorOfVectorOfPoints>& input_meshes,
                   const double dt,
                   const std::string& solver_type)
{
    SimulatorYamlParser parser(data);
    DataSourceBuilder builder(check_input_yaml(parser.parse()), input_meshes);
    DataSource ds = builder.build_ds();
    ds.set<double>("simulator_base_initial_time_step",dt);
    set_data_source_solver(ds, solver_type);
    return ds;
}
