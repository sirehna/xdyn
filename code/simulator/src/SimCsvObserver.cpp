/*
 * SimCsvObserver.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "SimCsvObserver.hpp"
#include "Sim.hpp"

SimCsvObserver::SimCsvObserver(std::ostream& os_) : os(os_), initialized(false), bodies(std::vector<std::string>())
{
}

std::string SimCsvObserver::customize(const std::string& body, const std::string anything) const
{
    return anything + "(" + body + ")";
}

void SimCsvObserver::observe(const Sim& sys, const double t)
{
    observe_states(sys, t);
    observe_waves(sys, t);
}

void SimCsvObserver::observe_waves(const Sim& , const double )
{

}

void SimCsvObserver::observe_states(const Sim& s, const double t)
{
    if (not(initialized)) initialize(s);
    initialized = true;
    const size_t n = bodies.size();
    os << t;
    if (n)
    {
        os << ',';
    }
    else
    {
        os << std::endl;
    }
    for (size_t i = 0 ; i < n ; ++i)
    {
        os << *_X(s.state,i) << ','
           << *_Y(s.state,i) << ','
           << *_Z(s.state,i) << ','
           << *_U(s.state,i) << ','
           << *_V(s.state,i) << ','
           << *_W(s.state,i) << ','
           << *_P(s.state,i) << ','
           << *_Q(s.state,i) << ','
           << *_R(s.state,i) << ','
           << *_QR(s.state,i) << ','
           << *_QI(s.state,i) << ','
           << *_QJ(s.state,i) << ','
           << *_QK(s.state,i);
        if (i == n-1)
        {
            os << std::endl;
        }
        else
        {
            os << ',';
        }
    }
}

void SimCsvObserver::initialize(const Sim& sys)
{
    bodies = sys.get_names_of_bodies();
    initialize_title();
}

void SimCsvObserver::initialize_title()
{
    const size_t n = bodies.size();
    os << "t";
    if (n)
    {
        os << ',';
    }
    else
    {
        os << std::endl;
    }
    for (size_t i = 0 ; i < n ; ++i)
    {
        os << customize(bodies[i],"x") << ','
           << customize(bodies[i],"y") << ','
           << customize(bodies[i],"z") << ','
           << customize(bodies[i],"u") << ','
           << customize(bodies[i],"v") << ','
           << customize(bodies[i],"w") << ','
           << customize(bodies[i],"p") << ','
           << customize(bodies[i],"q") << ','
           << customize(bodies[i],"r") << ','
           << customize(bodies[i],"qr") << ','
           << customize(bodies[i],"qi") << ','
           << customize(bodies[i],"qj") << ','
           << customize(bodies[i],"qk");
        if (i == n-1)
        {
            os << std::endl;
        }
        else
        {
            os << ',';
        }
    }
}
