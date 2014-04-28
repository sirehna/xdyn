/*
 * KinematicsModuleTest.cpp
 *
 *  Created on: Apr 25, 2014
 *      Author: cady
 */

#include "KinematicsModuleTest.hpp"
#include "DataSourceBuilder.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"
#include "Transform.hpp"
#include "Kinematics.hpp"
#include "transform_double_equal.hpp"


KinematicsModuleTest::KinematicsModuleTest() : a(DataGenerator(18914214))
{
}

KinematicsModuleTest::~KinematicsModuleTest()
{
}

void KinematicsModuleTest::SetUp()
{
}

void KinematicsModuleTest::TearDown()
{
}

TEST_F(KinematicsModuleTest, example)
{
//! [KinematicsModuleTest example]
	const SimulatorYamlParser parser(test_data::full_example());
	DataSourceBuilder builder(parser.parse());
	DataSource ds = builder.build_ds();
//! [KinematicsModuleTest example]
//! [KinematicsModuleTest expected output]
	const std::tr1::shared_ptr<Kinematics> k = ds.get<std::tr1::shared_ptr<Kinematics> >("kinematics");

	const kinematics::Transform T1 = k->get("body 1", "NED");
	const kinematics::Transform T2 = k->get("NED", "body 1");

	ASSERT_TRUE(double_equal(kinematics::identity("body 1"), T2*T1, 1E-10));
	ASSERT_TRUE(double_equal(kinematics::identity("NED"), T1*T2, 1E-10));
//! [KinematicsModuleTest expected output]
}

