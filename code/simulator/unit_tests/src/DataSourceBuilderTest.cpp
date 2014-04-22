/*
 * DataSourceBuilderTest.cpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */


#include "DataSourceBuilderTest.hpp"
#include "DataSourceBuilder.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"

DataSourceBuilderTest::DataSourceBuilderTest() : a(DataGenerator(12))
{
}

DataSourceBuilderTest::~DataSourceBuilderTest()
{
}

void DataSourceBuilderTest::SetUp()
{
}

void DataSourceBuilderTest::TearDown()
{
}

TEST_F(DataSourceBuilderTest, example)
{
//! [DataSourceBuilderTest example]
    const SimulatorYamlParser parser(test_data::full_example());
    DataSourceBuilder builder(parser);
    DataSource ds = builder.build_ds();
//! [DataSourceBuilderTest example]
//! [DataSourceBuilderTest expected output]
//! [DataSourceBuilderTest expected output]
}
