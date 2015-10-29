#include "ConfBuilder.hpp"
#include "simulator_api.hpp"



ConfBuilder::ConfBuilder(const std::string& yaml)
    : sim(get_system(yaml, 0, ssc::data_source::DataSource()))
    , Tmax(sim.get_bodies().front()->get_states().x.get_Tmax())
{
}
