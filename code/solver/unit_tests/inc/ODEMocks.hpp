/*
 * ODEMocks.hpp
 *
 *  Created on: 19 mars 2014
 *      Author: maroff
 */

#ifndef ODEMOCKS_HPP_
#define ODEMOCKS_HPP_


#include "gmock/gmock.h"

#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>
#include <vector>

class SystemWithMockButNoUpdate;

class ODEMocks
{
    public:
        ODEMocks(){}
        MOCK_METHOD3(model_function, void(const std::vector<double>&, std::vector<double>&, double ));
        MOCK_METHOD2(observe, void(const SystemWithMockButNoUpdate&, const double));
        MOCK_METHOD4(do_step, void(SystemWithMockButNoUpdate& sys, std::vector<double>& , double , double ));
        MOCK_METHOD4(try_step, boost::numeric::odeint::controlled_step_result(SystemWithMockButNoUpdate& sys, std::vector<double>& , double , double ));
        MOCK_CONST_METHOD0(has_more_time_events, bool());
        MOCK_CONST_METHOD0(get_time, double());
        MOCK_CONST_METHOD0(get_step, double());
        MOCK_CONST_METHOD0(detected_state_events, bool());
        MOCK_CONST_METHOD0(update_discrete_states, void());
        MOCK_CONST_METHOD0(update_continuous_states, void());
        MOCK_CONST_METHOD1(append_time_event, void(const double));
        MOCK_CONST_METHOD0(locate_event, void());
        MOCK_CONST_METHOD0(run_event_actions, void());
};

class SystemWithMockButNoUpdate
{
    public:
        SystemWithMockButNoUpdate(ODEMocks& mock_) : mock(mock_){}
        void operator()(const std::vector<double>& x, std::vector<double>& dx_dt, double t)
        {
            mock.model_function(x, dx_dt, t);
        }

        std::vector<double> state;

    protected:
        ODEMocks& mock;

    private:
        SystemWithMockButNoUpdate();


};

class SystemWithMockAndStateUpdate : public SystemWithMockButNoUpdate
{
    public:
        SystemWithMockAndStateUpdate(ODEMocks& mock_) : SystemWithMockButNoUpdate(mock_){}

        void update_discrete_states()
        {
            mock.update_discrete_states();
        }

        void update_continuous_states()
        {
            mock.update_continuous_states();
        }

    private:
        SystemWithMockAndStateUpdate();
};

template <typename SystemType> class ObserverWithMock
{
    public:
        ObserverWithMock(ODEMocks& mock_) : mock(mock_){}
        void observe(const SystemType& s, const double d)
        {
            mock.observe(s,d);
        }

    private:
        ObserverWithMock();
        ODEMocks& mock;
};

class SchedulerWithMock
{
    public:
        SchedulerWithMock(ODEMocks& mock_) : mock(mock_), first_run(new bool(true)){}
        bool has_more_time_events() const
        {
            mock.has_more_time_events();
            const bool b = *first_run;
            *first_run = false;
            return b;
        }

        void append_time_event(const double t)
        {
            mock.append_time_event(t);
        }

        ~SchedulerWithMock()
        {
            delete first_run;
        }

        SchedulerWithMock& operator=(const SchedulerWithMock& rhs)
        {
            if (this != &rhs)
            {
                *first_run = *(rhs.first_run);
            }
            return *this;
        }

        SchedulerWithMock(const SchedulerWithMock& rhs) : mock(rhs.mock), first_run(new bool(*(rhs.first_run)))
        {

        }

        double get_time() const
        {
            return mock.get_time();
        }

        double get_step() const
        {
            return mock.get_step();
        }

    private:
        SchedulerWithMock();
        ODEMocks& mock;
        bool* first_run;
};

class EventHandlerWithMock
{
    public:
        EventHandlerWithMock(ODEMocks& mock_) : mock(mock_){}

        bool detected_state_events()
        {
            return mock.detected_state_events();
        }

        void locate_event()
        {
            mock.locate_event();
        }

        void run_event_actions()
        {
            mock.run_event_actions();
        }

    private:
        EventHandlerWithMock();
        ODEMocks& mock;
};

class ExplicitStepperWithMock
{
    public:
        ExplicitStepperWithMock(ODEMocks& mock_) : mock(mock_){}
        void do_step(SystemWithMockButNoUpdate& sys, std::vector<double>& x, double t, double dt)
        {
            mock.do_step(sys,x,t,dt);
        }

    private:
        ExplicitStepperWithMock();
        ODEMocks& mock;
};

class ControlledStepperWithMock
{
    public:
        ControlledStepperWithMock(ODEMocks& mock_) : mock(mock_){}

        boost::numeric::odeint::controlled_step_result try_step(SystemWithMockButNoUpdate& sys, std::vector<double>& x, double t, double dt)
        {
            return mock.try_step(sys,x,t,dt);
        }


    private:
        ControlledStepperWithMock();
        ODEMocks& mock;
};


#endif /* ODEMOCKS_HPP_ */
