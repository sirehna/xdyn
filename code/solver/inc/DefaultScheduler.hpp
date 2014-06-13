/*
 * DefaultScheduler.hpp
 *
 *  Created on: 21 mars 2014
 *      Author: cec
 */

#ifndef DEFAULTSCHEDULER_HPP_
#define DEFAULTSCHEDULER_HPP_


class DefaultScheduler
{
    public:
        DefaultScheduler(const double t0_, const double tend_, const double dt_) : t(t0_), tstart(t0_), tend(tend_), dt(dt_), i(0) {}

        bool has_more_time_events() const
        {
            return tend > t;
        }

        void append_time_event(const double t_)
        {
            t = t_;
        }

        double get_time() const
        {
            return t;
        }

        double get_step() const
        {
            return dt;
        }

    private:
        DefaultScheduler();
        double t;
        double tstart;
        double tend;
        double dt;
        int i;
};



#endif /* DEFAULTSCHEDULER_HPP_ */
