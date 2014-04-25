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
#include "EulerAngles.hpp"
#include "rotation_matrix_builders.hpp"

#include <Eigen/Geometry>

DataSourceBuilderTest::DataSourceBuilderTest() : a(DataGenerator(12)), ds(DataSource())
{
	const SimulatorYamlParser parser(test_data::full_example());
	DataSourceBuilder builder(parser.parse());
	ds = builder.build_ds();
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

TEST_F(DataSourceBuilderTest, DataSource_should_contain_initial_states)
{
	ASSERT_DOUBLE_EQ(4,ds.get<double>("x(body 1)"));
	ASSERT_DOUBLE_EQ(8,ds.get<double>("y(body 1)"));
	ASSERT_DOUBLE_EQ(12,ds.get<double>("z(body 1)"));
	ASSERT_DOUBLE_EQ(-8,ds.get<double>("u(body 1)"));
	ASSERT_DOUBLE_EQ(-9,ds.get<double>("v(body 1)"));
	ASSERT_DOUBLE_EQ(14,ds.get<double>("w(body 1)"));
	ASSERT_DOUBLE_EQ(56,ds.get<double>("p(body 1)"));
	ASSERT_DOUBLE_EQ(7,ds.get<double>("q(body 1)"));
	ASSERT_DOUBLE_EQ(6,ds.get<double>("r(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_initial_quaternions)
{
	const EulerAngles expected(13,14,15);
	const Eigen::Quaternion<double> q(kinematics::rotation_matrix<kinematics::INTRINSIC, kinematics::ORDERED_BY_ANGLE, kinematics::CARDAN, 3, 2, 1>(expected));

	ASSERT_DOUBLE_EQ(q.w(),ds.get<double>("qr(body 1)"));
	ASSERT_DOUBLE_EQ(q.x(),ds.get<double>("qi(body 1)"));
	ASSERT_DOUBLE_EQ(q.y(),ds.get<double>("qj(body 1)"));
	ASSERT_DOUBLE_EQ(q.z(),ds.get<double>("qk(body 1)"));
}
