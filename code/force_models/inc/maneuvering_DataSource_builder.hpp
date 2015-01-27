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
    ssc::data_source::DataSource build_ds(const std::map<std::string, NodePtr>& nodes);
}



#endif /* MANEUVERING_DATASOURCE_BUILDER_HPP_ */
