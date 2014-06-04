#include "SimulatorYamlParser.hpp"
#include "DataSourceBuilder.hpp"
#include "check_input_yaml.hpp"
#include "SimulatorAPI.hpp"

DataSource make_ds(const std::string& data)
{
    SimulatorYamlParser parser(data);
    DataSourceBuilder builder(check_input_yaml(parser.parse()));
    return builder.build_ds();
}
