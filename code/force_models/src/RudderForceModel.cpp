/*
 * RudderForceModel.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: cady
 */

#include "RudderForceModel.hpp"

#include "external_data_structures_parsers.hpp"
#include "SurfaceElevationInterface.hpp"
#include "yaml2eigen.hpp"

#include <ssc/yaml_parser.hpp>
#include "yaml.h"

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define HYPOT(X,Y,Z) sqrt((X)*(X)+(Y)*(Y)+(Z)*(Z))

std::string RudderForceModel::model_name() {return "propeller+rudder";}

RudderForceModel::Yaml::Yaml() :
                Ar(0),
                b(0),
                effective_aspect_ratio_factor(0),
                lift_coeff(0),
                drag_coeff(0),
                position_of_the_rudder_frame_in_the_body_frame()
{
}

RudderForceModel::Yaml::Yaml(const WageningenControlledForceModel::Yaml& yaml) :
                        WageningenControlledForceModel::Yaml(yaml),
                        Ar(0),
                        b(0),
                        effective_aspect_ratio_factor(0),
                        lift_coeff(0),
                        drag_coeff(0),
                        position_of_the_rudder_frame_in_the_body_frame()
{
}

RudderForceModel::RudderModel::RudderModel(const Yaml& parameters_, const double rho_, const double nu_) :
                                           parameters(parameters_),
                                           chord(parameters.Ar/parameters.b),
                                           lambda(parameters.effective_aspect_ratio_factor * parameters.b*parameters.b / parameters.Ar),
                                           D(parameters_.diameter),
                                           Kr(),
                                           rho(rho_),
                                           nu(nu_),
                                           translation_from_rudder_to_propeller(parameters.position_of_propeller_frame.coordinates.x - parameters.position_of_the_rudder_frame_in_the_body_frame.x,
                                                                                parameters.position_of_propeller_frame.coordinates.y - parameters.position_of_the_rudder_frame_in_the_body_frame.y,
                                                                                parameters.position_of_propeller_frame.coordinates.z - parameters.position_of_the_rudder_frame_in_the_body_frame.z)
{
    const double distance_between_rudder_and_screw = std::abs(parameters_.position_of_propeller_frame.coordinates.x - parameters_.position_of_the_rudder_frame_in_the_body_frame.x);
    Kr = 0.5+0.5/(1+0.15/std::abs(distance_between_rudder_and_screw/parameters_.diameter));
}

double RudderForceModel::RudderModel::get_angle_of_attack(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                          const double fluid_angle   //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                          ) const
{
    return rudder_angle-fluid_angle;
}

double RudderForceModel::RudderModel::get_Cd(const double Vs, //!< Norm of the speed of the ship relative to the fluid
                                             const double Cl  //!< Rudder lift coefficient (non-dimensional)
                                             ) const
{
    // Reynolds number of the rudder (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 78 eq. 1.2.12)
    const double Rn = Vs * chord / nu;
    // ITTC resistance coefficient , "Marine rudders and Control Surfaces" p.31 eq. 3.18
    const double Cf = 0.075 / pow((log(Rn)/log(10.0)-2),2);
    // Resistance coefficient (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 78 (§ "for Cd0"))
    const double Cd0 = 2.5 * Cf;
    // Rudder drag coefficient (cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag, p. 78 eq. 1.2.9)
    return 1.1 * Cl*Cl / (PI * lambda) + Cd0;
}

double RudderForceModel::RudderModel::get_Cl(const double alpha_wake //!< Angle of rudder wrt the fluid (in radian)
                                            ) const
{
    return 2 * PI * lambda * (lambda + 1) / (lambda + 2) / (lambda + 2) * sin(alpha_wake);
}

double RudderForceModel::RudderModel::get_lift(const double Vs,//!< Norm of the speed of the ship relative to the fluid
                                               const double Cl,//!< Rudder lift coefficient (non-dimensional)
                                               const double alpha, //!< Angle between the propeller's wake & the rudder (in radian)
                                               const double area   //!< Rudder area (in or outside wake) in m^2
                                               ) const
{
    return 0.5 * rho * area * Vs*Vs * Cl * cos(alpha) * parameters.lift_coeff;
}

double RudderForceModel::RudderModel::get_drag(const double Vs,//!< Norm of the speed of the ship relative to the fluid
                                               const double Cl,//!< Rudder lift coefficient (non-dimensional)
                                               const double alpha, //!< Angle between the propeller's wake & the rudder (in radian)
                                               const double area   //!< Rudder area (in or outside wake) in m^2
                                               ) const
{
    return 0.5 * rho * area * Vs*Vs * Cl * parameters.drag_coeff;
}

ssc::kinematics::Vector6d RudderForceModel::RudderModel::get_force(const double lift, //!< Norm of the lift (in N)
                                                                   const double drag, //!< Norm of the drag (in N)
                                                                   const double angle //!< Angle between the fluid & the rudder (in radian)
                                    ) const
{
    ssc::kinematics::Vector6d ret = ssc::kinematics::Vector6d::Zero();
    const Eigen::Vector3d f(- lift * sin (angle) - drag * cos (angle),
                            + lift * cos (angle) - drag * sin (angle),
                            0);
    const Eigen::Vector3d m = -translation_from_rudder_to_propeller.cross(f);
    ret(0) = f(0);
    ret(1) = f(1);
    ret(2) = f(2);
    ret(3) = m(0);
    ret(4) = m(1);
    ret(5) = m(2);

    return ret;
}

RudderForceModel::InOutWake<ssc::kinematics::Point> RudderForceModel::RudderModel::get_vs(const double CTh, //!< Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
                                                                                          const double Va,  //!< Projection of the ship speed (relative to the current) on the X-axis of the ship's reference frame (m/s)
                                                                                          const double v,   //!< Projection of the ship speed (relative to the current) on the Y-axis of the ship's reference frame (m/s)
                                                                                          const double T    //!< Propeller thrust (in N)
                                                                                          ) const
{
    RudderForceModel::InOutWake<ssc::kinematics::Point> Vs;
    // Reduction factor (cf. "Marine rudders and Control Surfaces", p.371, eq 11.1)
    const double RF = CTh>13.71742 ? 0.5 : 1 - 0.135 * sqrt(CTh); // Because 13.71742 = pow(0.5/0.135,2) and 1 - 0.135 * sqrt(pow(0.5/0.135,2)) = 0.5
    // Vchange = Vbollard - Va (cf. "Marine rudders and Control Surfaces", p.51, eq 3.38)
    const double Vchange = sqrt(Va*Va + 8 / PI * T / (rho * parameters.diameter*parameters.diameter)) - Va;
    // Ship speed (relative to the current) in the ship's reference frame (m/s)
    Vs.in_wake.x() = (Va+Kr*Vchange) * RF;
    Vs.in_wake.y() = v;
    Vs.outside_wake.x() = Va;
    Vs.outside_wake.y() = v;
    return Vs;
}

RudderForceModel::InOutWake<double> RudderForceModel::RudderModel::get_fluid_angle(const RudderForceModel::InOutWake<ssc::kinematics::Point>& Vs   //!< Ship speed relative to the fluid, inside & outside wake
                                              ) const
{
    RudderForceModel::InOutWake<double> angle;
    angle.in_wake      = atan2(Vs.in_wake.y(), Vs.in_wake.x());
    angle.outside_wake = atan2(Vs.outside_wake.y(), Vs.outside_wake.x());
    return angle;
}

RudderForceModel::InOutWake<ssc::kinematics::Vector6d> RudderForceModel::RudderModel::get_wrench(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                                                                 const RudderForceModel::InOutWake<double>& fluid_angle,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                                                                 const RudderForceModel::InOutWake<ssc::kinematics::Point>& Vs,           //!< Norm of the speed of the ship relative to the fluid (in m/s)
                                                                                                 const RudderForceModel::InOutWake<double>& area          //!< Rudder area (in or outside wake) in m^2
                                                                    ) const
{
    RudderForceModel::InOutWake<ssc::kinematics::Vector6d> ret;
    ret.in_wake = get_wrench(rudder_angle, fluid_angle.in_wake, (double)Vs.in_wake.v.norm(), area.in_wake);
    ret.outside_wake = get_wrench(rudder_angle, fluid_angle.outside_wake, (double)Vs.outside_wake.v.norm(), area.outside_wake);
    return ret;
}

ssc::kinematics::Vector6d RudderForceModel::RudderModel::get_wrench(const double rudder_angle, //!< Rudder angle (in radian): positive if rudder on port side
                                                                    const double fluid_angle,  //!< Angle of the fluid in the ship's reference frame (0 if the fluid is propagating along -X, positive if fluid is coming from starboard)
                                                                    const double Vs,           //!< Norm of the speed of the ship relative to the fluid (in m/s)
                                                                    const double area          //!< Rudder area (in or outside wake) in m^2
                                                             ) const
{
    const double alpha = get_angle_of_attack(rudder_angle, fluid_angle);
    const double Cl = get_Cl(alpha);
    const double lift = get_lift(Vs, Cl, alpha, area);
    const double Cd = get_Cd(Vs, Cl);
    const double drag = get_drag(Vs, Cd, alpha, area);
    return get_force(lift, drag, fluid_angle);
}

RudderForceModel::InOutWake<double> RudderForceModel::RudderModel::get_Ar(const double CTh //!< Thrust loading coefficient, Cf. "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
                                                          ) const
{
    InOutWake<double> ar;
    // Jet speed coefficient, "Manoeuvring Technical Manual", J. Brix, Seehafen Verlag p. 96 eq. 1.2.44
    const double Cj = 1 + Kr * (sqrt(1 + CTh) -1);
    const double Dwake = D * sqrt((1 + 0.5 * (sqrt(1 + CTh) - 1)) / Cj);
    ar.in_wake = std::min(parameters.Ar, chord*Dwake);
    ar.outside_wake = parameters.Ar-ar.in_wake;
    return ar;
}

RudderForceModel::RudderForceModel(const Yaml& input_, const std::string& body_name_, const EnvironmentAndFrames& env_) :
        ControllableForceModel(input_.name,{"rpm","P/D","beta"},input_.position_of_propeller_frame, body_name_, env_),
        propulsion(WageningenControlledForceModel(input_, body_name_, env)),
        rudder_position(ssc::kinematics::Point(make_point(input_.position_of_propeller_frame.coordinates, input_.position_of_propeller_frame.frame))),
        model(input_, env_.rho, env_.nu),
        translation_from_rudder_to_propeller(rudder_position.get_frame(),
                                             input_.position_of_propeller_frame.coordinates.x - rudder_position.x(),
                                             input_.position_of_propeller_frame.coordinates.y - rudder_position.y(),
                                             input_.position_of_propeller_frame.coordinates.z - rudder_position.z()),
        w(input_.wake_coefficient)
{
}

ssc::kinematics::Vector6d RudderForceModel::get_rudder_force(const BodyStates& states, const double t, const std::map<std::string,double>& commands, const double T) const
{
    const double Va = states.u()*(1-w); // Cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag p. 96, eq. 1.2.41
    const double DVa = model.get_D()*Va;
    // Thrust loading coefficient, Cf. "Maneuvering Technical Manual", J. Brix, Seehafen Verlag p. 84, eq. 1.2.20
    const double CTh = std::abs(DVa) < 1e-10 ? 8e20 / PI * T / env.rho : 8 / PI * T / (env.rho * DVa*DVa);

    const double rudder_angle = commands.at("beta");
    const InOutWake<ssc::kinematics::Point> Vs = model.get_vs(CTh, Va, (double)states.v(), T);
    const InOutWake<double> fluid_angle = model.get_fluid_angle(Vs);
    const InOutWake<double> area = model.get_Ar(CTh);
    const InOutWake<ssc::kinematics::Vector6d> w = model.get_wrench(rudder_angle, fluid_angle, Vs, area);
    return w.in_wake + w.outside_wake;
}

ssc::kinematics::Vector6d RudderForceModel::get_force(const BodyStates& states, const double t, const std::map<std::string,double>& commands) const
{
    const ssc::kinematics::Vector6d propeller_force = propulsion.get_force(states,t,commands);
    const ssc::kinematics::Vector6d rudder_force = get_rudder_force(states, t, commands, (double)propeller_force.norm());
    const std::string frame = translation_from_rudder_to_propeller.get_frame();
    const ssc::kinematics::Wrench prop(frame, propeller_force);
    const ssc::kinematics::Wrench rudder_wrench(ssc::kinematics::Point(frame,0,0,0), rudder_force);
    rudder_wrench.change_point_of_application(translation_from_rudder_to_propeller);
    const ssc::kinematics::Wrench Ftot = rudder_wrench + prop;
    return Ftot.to_vector();
}

ssc::kinematics::Point RudderForceModel::get_ship_speed(const BodyStates& states, const double t) const
{
    const auto Tbody2ned = env.k->get(rudder_position.get_frame(),"NED");
    const ssc::kinematics::Point P_ = Tbody2ned*rudder_position;
    const ssc::kinematics::Point P("NED", -P.v);
    const std::vector<double> x{P.x()};
    const std::vector<double> y{P.y()};
    double eta = 0;
    try
    {
        eta = env.w->wave_height(x, y, t).at(0);
    }
    catch (const ssc::exception_handling::Exception& e)
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the propeller+rudder force model which uses the wave elevations to compute the orbital velocity on the rudder. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
    }
    try
    {
        ssc::kinematics::Point Vwater_ground;
        try
        {
            Vwater_ground = env.w->orbital_velocity(env.g, P.x(), P.y(), P.z(), t, eta);
        }
        catch (const ssc::exception_handling::Exception& e)
        {
            THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the propeller+rudder force model which needs the orbital velocity on the rudder. When querying the wave model for this information, the following problem occurred:\n" << e.get_message());
        }
        const ssc::kinematics::Point Vship_ground(rudder_position.get_frame(), states.u(), states.v(),states.w());
        const ssc::kinematics::Point Vship_water("NED", Vship_ground.x() - Vwater_ground.x(),
                                                        Vship_ground.y() - Vwater_ground.y(),
                                                        Vship_ground.z() - Vwater_ground.z());
        const auto Vwater_ground_projected_in_body = Tbody2ned.get_rot()*Vship_water.v;

        return ssc::kinematics::Point(rudder_position.get_frame(), Vwater_ground_projected_in_body);
    }
    catch (const ssc::exception_handling::Exception& e)
    {
        THROW(__PRETTY_FUNCTION__, ssc::exception_handling::Exception, "This simulation uses the propeller+rudder force model which needs the orbital velocities of the wave particles. During the evaluation of these orbital velocities, the following problem occurred:\n" << e.get_message());
    }
    return ssc::kinematics::Point();
}

RudderForceModel::Yaml RudderForceModel::parse(const std::string& yaml)
{
    std::stringstream stream(yaml);
    YAML::Parser parser(stream);
    YAML::Node node;
    parser.GetNextDocument(node);
    Yaml ret(WageningenControlledForceModel::parse(yaml));

    ssc::yaml_parser::parse_uv(node["rudder area"], ret.Ar);
    ssc::yaml_parser::parse_uv(node["rudder height"], ret.b);
    node["effective aspect ratio factor"]    >> ret.effective_aspect_ratio_factor;
    node["lift tuning coefficient"]          >> ret.lift_coeff;
    node["drag tuning coefficient"]          >> ret.drag_coeff;
    node["position of rudder in body frame"] >> ret.position_of_the_rudder_frame_in_the_body_frame;

    return ret;
}

double RudderForceModel::RudderModel::get_D() const
{
    return D;
}
