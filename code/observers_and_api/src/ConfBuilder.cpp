#include "ConfBuilder.hpp"
#include "simulator_api.hpp"

ssc::data_source::DataSource get_ds(const std::map<std::string, double>& commands);

ssc::data_source::DataSource get_ds(const std::map<std::string, double>& commands)
{
    ssc::data_source::DataSource ds;
    for (const auto c : commands)
    {
        ds.set(c.first, c.second);
    }
    return ds;
}


ConfBuilder::ConfBuilder(const std::string& yaml, const std::map<std::string,double>& commands)
    : sim(get_system(yaml, 0, get_ds(commands)))
    , Tmax(sim.get_bodies().front()->get_states().x.get_Tmax())
{
}
