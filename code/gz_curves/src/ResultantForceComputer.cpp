/*
 * ResultantForceComputer.cpp
 *
 *  Created on: Feb 9, 2015
 *      Author: cady
 */

#include "GZTypes.hpp"
#include "GravityForceModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "ResultantForceComputer.hpp"
#include "Sim.hpp"

#define NORM(f) (sqrt(f.X()*f.X()+f.Y()*f.Y()+f.Z()*f.Z()))

GZ::ResultantForceComputer::ResultantForceComputer(const Sim& s) :
    body(s.get_bodies().front()),
    env(s.get_env()),
    gravity(TR1(static_pointer_cast)<GravityForceModel>(s.get_forces().begin()->second.front())),
    hydrostatic(TR1(static_pointer_cast)<HydrostaticForceModel>(s.get_forces().begin()->second.back())),
    current_instant(0)
{

}

GZ::Resultant GZ::ResultantForceComputer::operator()(const ::GZ::State& point)
{
    std::vector<double> x(13, 0);
    x[ZIDX(0)] = point.z;
    ssc::kinematics::EulerAngles angle(point.phi,point.theta,0);
    YamlRotation c;
    c.order_by = "angle";
    c.convention.push_back("z");
    c.convention.push_back("y'");
    c.convention.push_back("x''");
    const auto quaternion = body->get_quaternions(angle, c);
    x[QRIDX(0)] = std::get<0>(quaternion);
    x[QIIDX(0)] = std::get<1>(quaternion);
    x[QJIDX(0)] = std::get<2>(quaternion);
    x[QKIDX(0)] = std::get<3>(quaternion);

    body->update(env,x,current_instant);

    auto sum_of_forces = gravity->operator ()(body->get_states(),current_instant)
                       + hydrostatic->operator ()(body->get_states(),current_instant);

    current_instant += 1;
    GZ::Resultant ret;
    ret.state = GZ::State(sum_of_forces.Z(), sum_of_forces.K(), sum_of_forces.M());
    ret.centre_of_buyoancy = hydrostatic->get_centre_of_buoyancy();

    return ret;
}

