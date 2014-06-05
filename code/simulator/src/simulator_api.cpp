#include "SimulatorYamlParser.hpp"
#include "DataSourceBuilder.hpp"
#include "check_input_yaml.hpp"
#include "simulator_api.hpp"

DataSource make_ds(const std::string& data)
{
    SimulatorYamlParser parser(data);
    DataSourceBuilder builder(check_input_yaml(parser.parse()));
    return builder.build_ds();
}

DataSource make_ds(const std::string& data,
                   const std::map<std::string, VectorOfVectorOfPoints>& input_meshes)
{
    SimulatorYamlParser parser(data);
    DataSourceBuilder builder(check_input_yaml(parser.parse()), input_meshes);
    return builder.build_ds();
}
