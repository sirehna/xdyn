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

GZ::ResultantForceComputer::ResultantForceComputer(const Sim& s, const double dz_, const double dphi_, const double dtheta_) :
    body(s.get_bodies().front()),
    env(s.get_env()),
    gravity(TR1(static_pointer_cast)<GravityForceModel>(s.get_forces().begin()->second.front())),
    hydrostatic(TR1(static_pointer_cast)<HydrostaticForceModel>(s.get_forces().begin()->second.back())),
    current_instant(0),
    G(body->get_states().G),
    dz(dz_),
    dphi(dphi_),
    dtheta(dtheta_)
{

}

GZ::Resultant GZ::ResultantForceComputer::resultant(const ::GZ::State& point)
{
    std::vector<double> x(13, 0);
    x[ZIDX(0)] = point(0);
    ssc::kinematics::EulerAngles angle(point(1),point(2),0);
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

double GZ::ResultantForceComputer::gz(const ssc::kinematics::Point& B //!< Centre of buoyancy in the body frame
                                ) const
{
    return (B-G)(1);
}

Eigen::Matrix2d GZ::ResultantForceComputer::dF(const Eigen::Vector3d& X)
{
    const Eigen::Vector3d dz_2(dz/2,0,0);
    const Eigen::Vector3d dtheta_2(0,0,dtheta/2);
    const Eigen::Vector3d dFz     = resultant(X+dz_2).state     - resultant(X-dz_2).state;
    const Eigen::Vector3d dFtheta = resultant(X+dtheta_2).state - resultant(X-dtheta_2).state;
    Eigen::Matrix2d F;
    F << dFz(0), dFtheta(0),
         dFz(2), dFtheta(2);
    return F;
}

Eigen::Matrix2d GZ::ResultantForceComputer::K(const Eigen::Vector3d& X)
{
    Eigen::Matrix2d ret;
    const Eigen::Vector2d dx(dz,dtheta);
    const Eigen::Matrix2d df = dF(X);

    for (size_t i = 0 ; i < 2 ; ++i)
    {
        for (size_t j = 0 ; j < 2 ; ++j)
        {
            ret(i,j) = df(i,j)/dx(j);
        }
    }
    return ret;
}

GZ::MinMax GZ::ResultantForceComputer::get_zmin_zmax(const double phi)
{
    std::vector<double> x(13, 0);
    ssc::kinematics::EulerAngles angle(phi, 0, 0);
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

    const auto Tmesh2ned = env.k->get(body->get_states().M->get_frame(), "NED");
    const auto M = Tmesh2ned*(*(body->get_states().M));
    const double zmin = M.m.row(2).minCoeff();
    const double zmax = M.m.row(2).maxCoeff();
    current_instant++;
    return MinMax(zmin,zmax);
}
