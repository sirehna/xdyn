/*
 * SimStepperTest.cpp
 *
 *  Created on: 13 oct. 2015
 *      Author: sirehna
 */

#include "InvalidInputException.hpp"
#include "SimStepperTest.hpp"
#include "SimStepper.hpp"
#include "yaml_data.hpp"
#include "YamlSimServerInputs.hpp"
#include "ConfBuilder.hpp"
#include "SimServerInputs.hpp"
#include <ssc/macros.hpp>
#define EPS 1E-8

SimStepperTest::SimStepperTest() : a(ssc::random_data_generator::DataGenerator(1234))
{
}

SimStepperTest::~SimStepperTest()
{
}

void SimStepperTest::SetUp()
{
}

void SimStepperTest::TearDown()
{
}

TEST_F(SimStepperTest, can_compute_one_step_with_euler_solver)
{
//! [SimStepperTest can_compute_one_step_with_euler_solver]
    const double g = 9.81;
    const std::string solver = "euler";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;
    const double t_end = t_start+Dt;

    ConfBuilder builder(test_data::falling_ball_example());
    SimStepper simstepper(builder, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0=4;
    const double y0=8;
    const double z0=12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    YamlSimServerInputs y;
    y.Dt = t_end - t_start;
    y.states = std::vector<YamlState>(1, YamlState(t_start, x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0));

    SimServerInputs infos(y, Dt);

    const std::vector<YamlState> res = simstepper.step(infos, Dt);
//! [SimStepperTest example]

//! [SimStepperTest expected output]
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(t_end,                    res.back().t, EPS);
    ASSERT_NEAR(x0+u0*t_end,              res.back().x, EPS);
    ASSERT_NEAR(y0,                       res.back().y, EPS);
    ASSERT_NEAR(z0+g*t_end*(t_end-1.)/2., res.back().z, EPS);
    ASSERT_NEAR(u0,                       res.back().u, EPS);
    ASSERT_NEAR(v0,                       res.back().v, EPS);
    ASSERT_NEAR(w0+g*t_end,               res.back().w, EPS);
    ASSERT_NEAR(0,                        res.back().p, EPS);
    ASSERT_NEAR(0,                        res.back().q, EPS);
    ASSERT_NEAR(0,                        res.back().r, EPS);
    ASSERT_NEAR(1,                        res.back().qr, EPS);
    ASSERT_NEAR(0,                        res.back().qi, EPS);
    ASSERT_NEAR(0,                        res.back().qj, EPS);
    ASSERT_NEAR(0,                        res.back().qk, EPS);
//! [SimStepperTest expected output]
}

TEST_F(SimStepperTest, can_compute_same_step_several_times)
{
//! [SimStepperTest can_compute_one_step_with_euler_solver]
    const double g = 9.81;
    const std::string solver = "euler";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;
    const double t_end = t_start+Dt;

    ConfBuilder builder(test_data::falling_ball_example());
    SimStepper simstepper(builder, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0=4;
    const double y0=8;
    const double z0=12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    YamlSimServerInputs y;
    y.Dt = t_end - t_start;
    y.states = std::vector<YamlState>(1, YamlState(t_start, x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0));
    const SimServerInputs infos(y, Dt);
    std::vector<YamlState> res = simstepper.step(infos, Dt);
    res = simstepper.step(infos, Dt);
//! [SimStepperTest can_compute_one_step_with_euler_solver]

//! [SimStepperTest can_compute_one_step_with_euler_solver output]
    ASSERT_EQ(11, res.size());
    ASSERT_NEAR(t_end,                    res.back().t, EPS);
    ASSERT_NEAR(x0+u0*t_end,              res.back().x, EPS);
    ASSERT_NEAR(y0,                       res.back().y, EPS);
    ASSERT_NEAR(z0+g*t_end*(t_end-1.)/2., res.back().z, EPS);
    ASSERT_NEAR(u0,                       res.back().u, EPS);
    ASSERT_NEAR(v0,                       res.back().v, EPS);
    ASSERT_NEAR(w0+g*t_end,               res.back().w, EPS);
    ASSERT_NEAR(0,                        res.back().p, EPS);
    ASSERT_NEAR(0,                        res.back().q, EPS);
    ASSERT_NEAR(0,                        res.back().r, EPS);
    ASSERT_NEAR(1,                        res.back().qr, EPS);
    ASSERT_NEAR(0,                        res.back().qi, EPS);
    ASSERT_NEAR(0,                        res.back().qj, EPS);
    ASSERT_NEAR(0,                        res.back().qk, EPS);

//! [SimStepperTest expected output]

}
TEST_F(SimStepperTest, wrong_solver_must_raise_exception)
{
//! [SimStepperTest wrong_solver_must_raise_exception]
    const std::string solver = "wrong solver";
    const double dt = 1.0;
    const double t_start = 0;
    const double Dt = 10;

    ConfBuilder builder(test_data::falling_ball_example());
    SimStepper simstepper(builder, solver, dt); // SimStepper's creator controls the stability of the numerical integration
    const double x0=4;
    const double y0=8;
    const double z0=12;
    const double u0 = 1;
    const double v0 = 0;
    const double w0 = 0;

    YamlSimServerInputs y;
    y.Dt = Dt;
    y.states = std::vector<YamlState>(1, YamlState(t_start, x0, y0 ,z0 ,u0 ,v0 ,w0 ,0 ,0 ,0 ,1 ,0 ,0 ,0));

//! [SimStepperTest wrong_solver_must_raise_exception]

//! [SimStepperTest wrong_solver_must_raise_exception output]
    ASSERT_THROW(simstepper.step(SimServerInputs(y, Dt), Dt), InvalidInputException);
//! [SimStepperTest expected output]
}

TEST_F(SimStepperTest, sim_update)
{
    ConfBuilder builder(test_data::falling_ball_example());
    Sim sim(builder.sim);
    const StateType x1 = {100.0, 200.0 ,300.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0};
    const StateType x2 = {400.0, 500.0 ,600.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0};

    sim.set_bodystates(State(AbstractStates<double>(4.0, 8.0 ,12.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,0.0));
    sim.get_bodies().front()->update_body_states(x1, 10.0);
    sim.set_bodystates(State(AbstractStates<double>(4.0, 8.0 ,12.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,0.0));
    sim.get_bodies().front()->update_body_states(x2, 10.0);
    sim.set_bodystates(State(AbstractStates<double>(5.0, 200.0 ,300.0 ,1.0 ,0.0 ,0.0 ,0 ,0 ,0 ,1 ,0 ,0 ,0) ,0.0));
    ASSERT_EQ(sim.get_bodies().front()->get_states().x(), 5.0);
}

