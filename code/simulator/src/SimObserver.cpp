/*
 * SimObserver.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "SimObserver.hpp"
#include "StateMacros.hpp"
#include "Sim.hpp"
#include "YamlSimulatorInput.hpp"

SimObserver::SimObserver(const YamlSimulatorInput& input) : bodies(std::vector<std::string>()),
res(std::vector<std::map<std::string,double> >())
{
    for (const auto body:input.bodies) bodies.push_back(body.name);
}

void SimObserver::observe(const Sim& s, const double t)
{
    std::map<std::string,double> m;
    m["t"] = t;
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        m[customize(bodies[i],"x")] = *_X(s.state,i);
        m[customize(bodies[i],"y")] = *_Y(s.state,i);
        m[customize(bodies[i],"z")] = *_Z(s.state,i);
        m[customize(bodies[i],"u")] = *_U(s.state,i);
        m[customize(bodies[i],"v")] = *_V(s.state,i);
        m[customize(bodies[i],"w")] = *_W(s.state,i);
        m[customize(bodies[i],"p")] = *_P(s.state,i);
        m[customize(bodies[i],"q")] = *_Q(s.state,i);
        m[customize(bodies[i],"r")] = *_R(s.state,i);
        m[customize(bodies[i],"qr")] = *_QR(s.state,i);
        m[customize(bodies[i],"qi")] = *_QI(s.state,i);
        m[customize(bodies[i],"qj")] = *_QJ(s.state,i);
        m[customize(bodies[i],"qk")] = *_QK(s.state,i);
    }
    res.push_back(m);
}

std::string SimObserver::customize(const std::string& body, const std::string anything) const
{
    return anything + "(" + body + ")";
}

std::vector<std::map<std::string,double> > SimObserver::get() const
{
    return res;
}
