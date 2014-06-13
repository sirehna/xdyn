/*
 * constant_step_stepper_arcitecture_tests.cpp
 *
 *  Created on: 21 mars 2014
 *      Author: cec
 */

#include "constant_step_stepper_architecture_tests.hpp"
#include "ODEMocks.hpp"
#include "solve.hpp"

using ::testing::Return;
using ::testing::StrictMock;
using ::testing::NiceMock;
using ::testing::InSequence;
using ::testing::_;

constant_step_stepper_architecture_tests::constant_step_stepper_architecture_tests() : a(DataGenerator(12212))
{
}

constant_step_stepper_architecture_tests::~constant_step_stepper_architecture_tests()
{
}

void constant_step_stepper_architecture_tests::SetUp()
{
}

void constant_step_stepper_architecture_tests::TearDown()
{
}

TEST_F(constant_step_stepper_architecture_tests, collaborators_should_be_called_in_the_right_sequence_for_constant_step_steppers_event_detected_with_state_update)
{
    StrictMock<ODEMocks> mock;
    SystemWithMockAndStateUpdate sys(mock);
    ObserverWithMock<SystemWithMockAndStateUpdate> observer(mock);
    SchedulerWithMock scheduler(mock);
    EventHandlerWithMock event_handler(mock);
    ExplicitStepperWithMock stepper(mock);

    ON_CALL(mock, get_time()).WillByDefault(Return(a.random<double>()()));
    ON_CALL(mock, detected_state_events()).WillByDefault(Return(true)); // Event detected

    InSequence seq; // Name is irrelevant: all expectations in its scope are expected to occur in sequence
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_step()).RetiresOnSaturation();
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, do_step(_,_,_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, detected_state_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, locate_event()).RetiresOnSaturation();
    EXPECT_CALL(mock, run_event_actions()).RetiresOnSaturation();
    EXPECT_CALL(mock, update_discrete_states()).RetiresOnSaturation();
    EXPECT_CALL(mock, update_continuous_states()).RetiresOnSaturation();
    EXPECT_CALL(mock, append_time_event(_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_));
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();

    solve_for_constant_step<ExplicitStepperWithMock,SystemWithMockAndStateUpdate,ObserverWithMock<SystemWithMockAndStateUpdate>,SchedulerWithMock,EventHandlerWithMock>(sys,observer,stepper,scheduler,event_handler);
}

TEST_F(constant_step_stepper_architecture_tests, collaborators_should_be_called_in_the_right_sequence_for_constant_step_steppers_event_detected_without_state_update)
{
    StrictMock<ODEMocks> mock;
    SystemWithMockButNoUpdate sys(mock);
    ObserverWithMock<SystemWithMockButNoUpdate> observer(mock);
    SchedulerWithMock scheduler(mock);
    EventHandlerWithMock event_handler(mock);
    ExplicitStepperWithMock stepper(mock);

    ON_CALL(mock, get_time()).WillByDefault(Return(a.random<double>()()));
    ON_CALL(mock, detected_state_events()).WillByDefault(Return(true)); // Event detected

    InSequence seq; // Name is irrelevant: all expectations in its scope are expected to occur in sequence

    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_step()).RetiresOnSaturation();
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, do_step(_,_,_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, detected_state_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, locate_event()).RetiresOnSaturation();
    EXPECT_CALL(mock, run_event_actions()).RetiresOnSaturation();
    EXPECT_CALL(mock, append_time_event(_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_));
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();


    solve_for_constant_step<ExplicitStepperWithMock,SystemWithMockButNoUpdate,ObserverWithMock<SystemWithMockButNoUpdate>,SchedulerWithMock,EventHandlerWithMock>(sys,observer,stepper,scheduler,event_handler);
}

TEST_F(constant_step_stepper_architecture_tests, collaborators_should_be_called_in_the_right_sequence_for_constant_step_steppers_no_event_detected_with_state_update)
{
    StrictMock<ODEMocks> mock;
    SystemWithMockAndStateUpdate sys(mock);
    ObserverWithMock<SystemWithMockAndStateUpdate> observer(mock);
    SchedulerWithMock scheduler(mock);
    EventHandlerWithMock event_handler(mock);
    ExplicitStepperWithMock stepper(mock);


    ON_CALL(mock, get_time()).WillByDefault(Return(a.random<double>()()));
    ON_CALL(mock, detected_state_events()).WillByDefault(Return(false)); // No event detected

    InSequence seq; // Name is irrelevant: all expectations in its scope are expected to occur in sequence
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_step()).RetiresOnSaturation();
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, do_step(_,_,_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, detected_state_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, update_discrete_states()).RetiresOnSaturation();
    EXPECT_CALL(mock, update_continuous_states()).RetiresOnSaturation();
    EXPECT_CALL(mock, append_time_event(_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_));
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();

    solve_for_constant_step<ExplicitStepperWithMock,SystemWithMockAndStateUpdate,ObserverWithMock<SystemWithMockAndStateUpdate>,SchedulerWithMock,EventHandlerWithMock>(sys,observer,stepper,scheduler,event_handler);
}

TEST_F(constant_step_stepper_architecture_tests, collaborators_should_be_called_in_the_right_sequence_for_constant_step_steppers_no_event_detected_without_state_update)
{
    StrictMock<ODEMocks> mock;
    SystemWithMockButNoUpdate sys(mock);
    ObserverWithMock<SystemWithMockButNoUpdate> observer(mock);
    SchedulerWithMock scheduler(mock);
    EventHandlerWithMock event_handler(mock);
    ExplicitStepperWithMock stepper(mock);


    ON_CALL(mock, get_time()).WillByDefault(Return(a.random<double>()()));
    ON_CALL(mock, detected_state_events()).WillByDefault(Return(false)); // No event detected

    InSequence seq; // Name is irrelevant: all expectations in its scope are expected to occur in sequence
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_step()).RetiresOnSaturation();
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, do_step(_,_,_,_)).RetiresOnSaturation();
    EXPECT_CALL(mock, detected_state_events()).RetiresOnSaturation();
    EXPECT_CALL(mock, append_time_event(_)).RetiresOnSaturation();
    EXPECT_CALL(mock, get_time()).RetiresOnSaturation();
    EXPECT_CALL(mock, observe(_,_));
    EXPECT_CALL(mock, has_more_time_events()).RetiresOnSaturation();

    solve_for_constant_step<ExplicitStepperWithMock,SystemWithMockButNoUpdate,ObserverWithMock<SystemWithMockButNoUpdate>,SchedulerWithMock,EventHandlerWithMock>(sys,observer,stepper,scheduler,event_handler);
}
