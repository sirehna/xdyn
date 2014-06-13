/*
 * solve.hpp
 *
 *  Created on: 21 mars 2014
 *      Author: cec
 */

#ifndef SOLVE_HPP_
#define SOLVE_HPP_

#include "DefaultEventHandler.hpp"
#include "DefaultScheduler.hpp"
#include "detection_of_state_update_methods.hpp"

#include <boost/numeric/odeint/stepper/controlled_step_result.hpp>

template <typename StepperType,
          typename SystemType,
          typename ObserverType,
          typename SchedulerType,
          typename EventHandlerType>
void
solve_for_constant_step(SystemType& sys, ObserverType& observer, StepperType& stepper, SchedulerType& scheduler, EventHandlerType& event_handler)
{
    const double tstart = scheduler.get_time();
    observer.observe(sys,tstart);
    int i = 0;
    const double dt = scheduler.get_step();
    while(scheduler.has_more_time_events())
    {
        const double t = scheduler.get_time();
        stepper.do_step(sys, sys.state, t, dt);
        if (event_handler.detected_state_events())
        {
            event_handler.locate_event();
            event_handler.run_event_actions();
        }
        update<SystemType, can<SystemType>::update_discrete_and_continuous_states>::if_possible(sys);
        scheduler.append_time_event(tstart + (++i)*dt);
        observer.observe(sys, scheduler.get_time());
    }
}

template <typename StepperType,
          typename SystemType,
          typename ObserverType,
          typename SchedulerType,
          typename EventHandlerType>
void
solve_for_adaptive_step(SystemType& sys, ObserverType& observer, StepperType& stepper, SchedulerType& scheduler, EventHandlerType& event_handler)
{
    const double tstart = scheduler.get_time();
    observer.observe(sys,tstart);
    while(scheduler.has_more_time_events())
    {
        const double t = scheduler.get_time();
        const double dt = scheduler.get_step();
        const boost::numeric::odeint::controlled_step_result res = stepper.try_step(sys, sys.state, t, dt);
        if (res == boost::numeric::odeint::success)
        {
            if (event_handler.detected_state_events())
            {
                event_handler.locate_event();
                event_handler.run_event_actions();
            }
            update<SystemType, can<SystemType>::update_discrete_and_continuous_states>::if_possible(sys);
            scheduler.append_time_event(t+dt);
            observer.observe(sys, t);
        }
        else
        {
            scheduler.append_time_event(t+dt);
        }
    }
}

template <typename StepperType,
          typename SystemType,
          typename ObserverType>
void quicksolve(SystemType& sys, const double t0, const double tend, double dt, ObserverType& observer)
{
    StepperType stepper;
    DefaultScheduler scheduler(t0, tend, dt);
    DefaultEventHandler event_handler;
    solve_for_constant_step<StepperType,SystemType,ObserverType,DefaultScheduler,DefaultEventHandler>(sys,observer,stepper,scheduler,event_handler);
}

#endif /* SOLVE_HPP_ */
