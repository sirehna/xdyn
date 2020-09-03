/*
 * solver.hpp
 *
 *  Created on: Aug 25, 2020
 *      Author: cady
 */

#ifndef CORE_INC_SOLVER_HPP_
#define CORE_INC_SOLVER_HPP_

/* This file was created because forced states don't work properly in xdyn:
 * forced state values are indeed written to the observers & fed to the force
 * models, but unforced states are integrated by the solver & when the states
 * are no longer forced the system behaves as if no states had ever been forced.
 * Instead of fixing this in the SSC, which requires pushing to GitHub, updating
 * the Docker base images, etc. we created this file, which almost duplicates
 * what is done in the SSC's solve.hpp but accounts for forced values.
 */

#include <ssc/solver.hpp>
#include "Sim.hpp"

template <typename StepperType,
          typename ObserverType,
          typename StateForcer>
void quicksolve(Sim& sys, const double t0, const double tend, double dt, ObserverType& observer, StateForcer& force_states)
{
    StepperType stepper;
    ssc::solver::DefaultScheduler scheduler(t0, tend, dt);
    ssc::solver::DefaultEventHandler event_handler;
    const double tstart = scheduler.get_time();
    observer.observe(sys,tstart);
    int i = 0;
    while(scheduler.has_more_time_events())
    {
        const double t = scheduler.get_time();
        stepper.do_step(sys, sys.state, t, dt);
        force_states(sys.state, t);
        if (event_handler.detected_state_events())
        {
            event_handler.locate_event();
            event_handler.run_event_actions();
        }
        ssc::solver::update<Sim, ssc::solver::can<Sim>::update_discrete_and_continuous_states>::if_possible(sys);
        scheduler.append_time_event(tstart + (++i)*dt);
        observer.observe(sys, scheduler.get_time());
    }
}

#endif /* CORE_INC_SOLVER_HPP_ */
