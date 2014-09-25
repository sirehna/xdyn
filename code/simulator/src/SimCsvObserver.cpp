/*
 * SimCsvObserver.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "SimCsvObserver.hpp"
#include "Sim.hpp"

std::ostream dev_null_buffer(0);

SimCsvObserver::SimCsvObserver(std::ostream& simulation_stream_, std::ostream& wave_stream_) : simulation_stream(simulation_stream_),
wave_stream(wave_stream_), initialized(false), bodies(std::vector<std::string>())
{
}

SimCsvObserver::SimCsvObserver(std::ostream& simulation_stream_) : simulation_stream(simulation_stream_),
wave_stream(dev_null_buffer), initialized(false), bodies(std::vector<std::string>())
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
    initialized = true;
}

void SimCsvObserver::observe_waves(const Sim& sys, const double t)
{
    const std::vector<ssc::kinematics::Point> free_surface = sys.get_waves(t);
    if (not(initialized)) initialize_wave_output_stream(free_surface);
    if (not(free_surface.empty()))
    {
        wave_stream << "    - t: " << t << std::endl;
        if (free_surface.front().get_frame() != "NED")
        {
            wave_stream << "      x: [";
            for (size_t i = 0 ; i < free_surface.size()-1 ; ++i)
            {
                wave_stream << free_surface[i].x() << ",";
            }
            wave_stream << free_surface.back().x() << "]\n";
            wave_stream << "      y: [";
            for (size_t i = 0 ; i < free_surface.size()-1 ; ++i)
            {
                wave_stream << free_surface[i].y() << ",";
            }
            wave_stream << free_surface.back().y() << "]\n";
        }
        wave_stream << "      z: [";
        for (size_t i = 0 ; i < free_surface.size()-1 ; ++i)
        {
            wave_stream << free_surface[i].z() << ",";
        }
        wave_stream << free_surface.back().z() << "]\n";
    }
}

void SimCsvObserver::observe_states(const Sim& s, const double t)
{
    if (not(initialized)) initialize_simulation_output_stream(s);
    const size_t n = bodies.size();
    simulation_stream << t;
    if (n)
    {
        simulation_stream << ',';
    }
    else
    {
        simulation_stream << std::endl;
    }
    for (size_t i = 0 ; i < n ; ++i)
    {
        simulation_stream << *_X(s.state,i) << ','
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
            simulation_stream << std::endl;
        }
        else
        {
            simulation_stream << ',';
        }
    }
}

void SimCsvObserver::initialize_simulation_output_stream(const Sim& sys)
{
    bodies = sys.get_names_of_bodies();
    initialize_title();
}

void SimCsvObserver::initialize_wave_output_stream(const std::vector<ssc::kinematics::Point>& free_surface)
{
    if (not(free_surface.empty()))
    {
        wave_stream << "waves:\n";
        if (free_surface.front().get_frame() == "NED")
        {
            wave_stream << "  x: [";
            for (size_t i = 0 ; i < free_surface.size()-1 ; ++i)
            {
                wave_stream << free_surface[i].x() << ",";
            }
            wave_stream << free_surface.back().x() << "]\n";
            wave_stream << "  y: [";
            for (size_t i = 0 ; i < free_surface.size()-1 ; ++i)
            {
                wave_stream << free_surface[i].y() << ",";
            }
            wave_stream << free_surface.back().y() << "]\n";
        }
        wave_stream << "  timesteps:\n";
    }
}

void SimCsvObserver::initialize_title()
{
    const size_t n = bodies.size();
    simulation_stream << "t";
    if (n)
    {
        simulation_stream << ',';
    }
    else
    {
        simulation_stream << std::endl;
    }
    for (size_t i = 0 ; i < n ; ++i)
    {
        simulation_stream << customize(bodies[i],"x") << ','
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
            simulation_stream << std::endl;
        }
        else
        {
            simulation_stream << ',';
        }
    }
}
