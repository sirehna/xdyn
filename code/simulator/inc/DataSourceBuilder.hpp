/*
 * DataSourceBuilder.hpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */

#ifndef DATASOURCEBUILDER_HPP_
#define DATASOURCEBUILDER_HPP_

#include "DataSource.hpp"

class SimulatorYamlParser;

class DataSourceBuilder
{
    public:
        DataSourceBuilder(const SimulatorYamlParser& parser);
        DataSource build_ds();
};

#endif /* DATASOURCEBUILDER_HPP_ */
