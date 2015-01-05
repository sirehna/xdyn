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
wave_stream(wave_stream_), initialized(false), bodies(), forces()
{
}

SimCsvObserver::SimCsvObserver(std::ostream& simulation_stream_) : simulation_stream(simulation_stream_),
wave_stream(dev_null_buffer), initialized(false), bodies(), forces()
{
}

std::string SimCsvObserver::customize(const std::string& body, const std::string anything) const
{
    return anything + "(" + body + ")";
}

void SimCsvObserver::observe(const Sim& sys, const double t)
{
    if (not(initialized)) initialize_simulation_output_stream(sys);
    observe_states(sys, t);
    observe_forces(sys);
    simulation_stream << std::endl;
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

std::ostream& operator<< (std::ostream& stream, const ssc::kinematics::Vector6d& m);
std::ostream& operator<< (std::ostream& stream, const ssc::kinematics::Vector6d& m)
{
    for (size_t i=0;i<5;++i)
        stream << m(i)<<",";
    stream << m(5);
    return stream;
}

void SimCsvObserver::observe_forces(const Sim& sys)
{
    const auto f = sys.get_forces_as_a_vector_of_doubles();
    if (not(f.empty())) simulation_stream << ',';
    const size_t n = f.size();
    size_t i = 0;
    for (auto it = f.begin() ; it != f.end() ; ++it)
    {
        simulation_stream << *it;
        if (i < (n-1))
        {
            simulation_stream << ",";
        }
        ++i;
    }
}

void SimCsvObserver::observe_states(const Sim& s, const double t)
{
    const size_t n = bodies.size();
    simulation_stream << t;
    if (n)
    {
        simulation_stream << ',';
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
        if (i != n-1)
        {
            simulation_stream << ',';
        }
    }
}

void SimCsvObserver::initialize_simulation_output_stream(const Sim& sys)
{
    bodies = sys.get_names_of_bodies();
    auto force_names = sys.get_force_names();
    for(auto it = force_names.begin();it != force_names.end();++it)
    {
        forces.push_back("Fx(" + *it + ")");
        forces.push_back("Fy(" + *it + ")");
        forces.push_back("Fz(" + *it + ")");
        forces.push_back("Mx(" + *it + ")");
        forces.push_back("My(" + *it + ")");
        forces.push_back("Mz(" + *it + ")");
    }
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
        if (i != n-1)
        {
            simulation_stream << ',';
        }
    }
    if (not(forces.empty())) simulation_stream << ',';
    for (size_t i = 0 ; i < forces.size() ; ++i)
    {
        simulation_stream << forces[i];
        if (i != forces.size()-1)
        {
            simulation_stream << ',';
        }
    }
    simulation_stream << std::endl;
}
