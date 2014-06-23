/*
 * SimTest.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#include "DefaultWaveModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "SimObserver.hpp"
#include "Sim.hpp"
#include "SimTest.hpp"
#include "SimulatorBuilder.hpp"
#include "YamlSimulatorInput.hpp"

SimTest::SimTest() : a(DataGenerator(42222))
{
}

SimTest::~SimTest()
{
}

void SimTest::SetUp()
{
}

void SimTest::TearDown()
{
}

TEST_F(SimTest, example)
{
//! [SimTest example]
    YamlSimulatorInput input;
    SimulatorBuilder builder(input);
    std::map<std::string, VectorOfVectorOfPoints> mesh;

    builder.can_parse<HydrostaticForceModel>()
           .can_parse<DefaultWaveModel>();
    Sim sim = builder.build(mesh);
    SimObserver observer(sim.get_names_of_bodies());

    std::vector<double> x(13,0), dx_dt(13,0);

    sim(x,dx_dt,10);
//! [SimTest example]
//! [SimTest expected output]
//! [SimTest expected output]
}
