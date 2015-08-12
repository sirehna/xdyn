/*
 * maneuvering_DataSource_builder.cpp
 *
 *  Created on: Jan 27, 2015
 *      Author: cady
 */

#include "maneuvering_DataSource_builder.hpp"

namespace maneuvering
{
    class Module : public ssc::data_source::DataSourceModule
    {
        public:
            Module(ssc::data_source::DataSource* const data_source, const std::pair<std::string,NodePtr>& name_node) :
                ssc::data_source::DataSourceModule(data_source, name_node.first+"computer"),
                f(name_node.second->get_lambda()),
                output_var(name_node.first)
            {
            }

            Module(const Module& rhs, ssc::data_source::DataSource* const data_source) : ssc::data_source::DataSourceModule(rhs, data_source)
            {
            }

            ssc::data_source::DataSourceModule* clone() const
            {
                return new Module(*this);
            }

            ssc::data_source::DataSourceModule* clone(ssc::data_source::DataSource* const data_source) const
            {
                return new Module(*this, data_source);
            }

            void update() const
            {
                const BodyStates states = ds->get<BodyStates>("states");
                const double t = ds->get<double>("t");
                double ret = 0;
                if (not(ds->read_only())) ret = f(states, *ds, t);
                ds->set<double>(output_var, ret);
            }

        private:
            Function f;
            std::string output_var;

    };
}

maneuvering::Module make_module(ssc::data_source::DataSource& ds, const std::pair<std::string,maneuvering::NodePtr>& name_node);
maneuvering::Module make_module(ssc::data_source::DataSource& ds, const std::pair<std::string,maneuvering::NodePtr>& name_node)
{
    return maneuvering::Module(&ds, name_node);
}

void maneuvering::build_ds(ssc::data_source::DataSource& ds, const std::map<std::string, NodePtr>& nodes, const std::map<std::string, double>& commands)
{
    for (auto node:nodes) ds.add(make_module(ds,node));
    for (auto c:commands) ds.set<double>(c.first, c.second);
}
