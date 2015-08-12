/*
 * maneuvering_DataSource_builder.hpp
 *
 *  Created on: Jan 27, 2015
 *      Author: cady
 */

#ifndef MANEUVERING_DATASOURCE_BUILDER_HPP_
#define MANEUVERING_DATASOURCE_BUILDER_HPP_

#include <ssc/data_source.hpp>

#include "ManeuveringInternal.hpp"

namespace maneuvering
{
    void build_ds(ssc::data_source::DataSource& ds, const std::map<std::string, NodePtr>& nodes, const std::map<std::string, double>& commands=std::map<std::string, double>());
}



#endif /* MANEUVERING_DATASOURCE_BUILDER_HPP_ */
