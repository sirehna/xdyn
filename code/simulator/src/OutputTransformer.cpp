/*
 * OutputGenerator.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */
#include <ssc/kinematics.hpp>

#include "SurfaceElevationInterface.hpp"
#include "OutputTransformer.hpp"
#include "update_kinematics.hpp"
#include "SimulatorBuilder.hpp"
#include "OutputTransformerException.hpp"

OutputTransformer::OutputTransformer(const SimulatorBuilder& builder) :
            input(builder.get_parsed_yaml()),
            bodies(std::vector<Body>()),
            points(std::map<std::string,ssc::kinematics::Point>()),
            k(TR1(shared_ptr)<ssc::kinematics::Kinematics>(new ssc::kinematics::Kinematics())),
            forces(),
            env()
{
    bodies = builder.get_bodies(builder.make_mesh_map());
    env = builder.get_environment_and_frames(bodies);
    forces = builder.get_forces(env);
    for (auto that_point = input.points.begin() ; that_point != input.points.end() ; ++that_point)
    {
        points[that_point->name] = ssc::kinematics::Point(that_point->frame,that_point->x,that_point->y,that_point->z);
    }
}

void OutputTransformer::update_kinematics(const StateType& x) const
{
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        ::update_kinematics(x, bodies[i], i, k);
        ::update_kinematics(x, bodies[i], i, env.k);
    }
}

template <typename T> double get_axis_value(const T& P, const std::string& axis)
{
    if (axis=="x") return P.x();
    if (axis=="y") return P.y();
    if (axis=="z") return P.z();
                   return 0;
}

template <> double get_axis_value<ssc::kinematics::EulerAngles>(const ssc::kinematics::EulerAngles& P, const std::string& axis)
{
    if (axis=="x") return P.phi;
    if (axis=="y") return P.theta;
    if (axis=="z") return P.psi;
                   return 0;
}

std::string angle_name(const std::string& axis);
std::string angle_name(const std::string& axis)
{
    if (axis=="x") return "phi";
    if (axis=="y") return "theta";
    if (axis=="z") return "psi";
                   return "";
}

void OutputTransformer::fill(std::map<std::string,double>& out, const YamlPositionOutput& position) const
{
    const auto that_point = points.find(position.point);
    if (that_point != points.end())
    {
        const ssc::kinematics::Point O1P = that_point->second;
        const auto b1 = O1P.get_frame();
        const auto b2 = position.relative_to_frame;
        const auto b3 = position.projected_in_frame;
        const ssc::kinematics::Transform T13 = k->get(b1, b3);
        const auto T12 = k->get(b1, b2);
        const ssc::kinematics::Point O1O2 = T12.get_point();
        const ssc::kinematics::Point P = ssc::kinematics::Point(b3,T13.get_rot()*(O1P - O1O2));
        for (auto that_axis=position.axes.begin() ; that_axis != position.axes.end() ; ++that_axis)
        {
            const auto varname = *that_axis + "(" + that_point->first + " in " + b1 + " / " + b2 + " -> " + b3 + ")";
            out[varname] = get_axis_value(P, *that_axis);
        }
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, OutputTransformerException, std::string("Unable to find point '") + position.point + "' in input data");
    }
}

bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third);
bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third)
{
    return (convention.at(0) == first) and (convention.at(1) == second) and (convention.at(2) == third);
}

ssc::kinematics::EulerAngles OutputTransformer::convert(const ssc::kinematics::RotationMatrix& R) const
{
    using namespace ssc::kinematics;
    if (input.rotations.order_by == "angle")
    {
        if (match(input.rotations.convention, "z", "y'", "x''"))
            return euler_angles<INTRINSIC, CHANGING_ANGLE_ORDER, CARDAN, 3, 2, 1>(R);
        std::stringstream ss;
        ss << "Rotation convention '" << input.rotations.convention.at(0) << "," << input.rotations.convention.at(1) << "," << input.rotations.convention.at(2) << "' is not currently supported.";
        THROW(__PRETTY_FUNCTION__, OutputTransformerException, ss.str());
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, OutputTransformerException, std::string("Ordering rotations by '") + input.rotations.order_by + "' is not currently supported");
    }
    return EulerAngles();
}

void OutputTransformer::fill(std::map<std::string,double>& out, const YamlAnglesOutput& angle) const
{
    using namespace ssc::kinematics;
    const auto b1 = angle.frame;
    const auto b2 = angle.relative_to_frame;
    const auto b3 = angle.projected_in_frame;
    const auto R13 = k->get(b1, b3).get_rot();
    const auto R12 = k->get(b1, b2).get_rot();
    RotationMatrix R12_projected_in_3 = R13.transpose();
    R12_projected_in_3 *= R12.transpose();
    R12_projected_in_3 *= R13;
    const EulerAngles a = convert(R12_projected_in_3);
    for (auto that_axis=angle.axes.begin() ; that_axis != angle.axes.end() ; ++that_axis)
    {
        const auto varname = angle_name(*that_axis) + "(" + b1 + " / " + b2 + " -> " + b3 + ")";
        out[varname] = get_axis_value(a, *that_axis);
    }
}

double OutputTransformer::compute_kinetic_energy(const size_t i, const StateType& x) const
{
    Eigen::VectorXd V(6);

    V(0) = *_U(x,i);
    V(1) = *_V(x,i);
    V(2) = *_W(x,i);
    V(3) = *_P(x,i);
    V(4) = *_Q(x,i);
    V(5) = *_R(x,i);

    const auto IV = bodies.at(i).solid_body_inertia->operator*(V);

    return 0.5*(V.transpose()*IV)(0,0);
}

double OutputTransformer::compute_potential_energy(const size_t i, const StateType& x) const
{
    double Ep = 0;
    for (auto that_force = forces.at(i).begin() ; that_force != forces.at(i).end() ; ++that_force)
    {
        const double ep = (*that_force)->potential_energy(bodies.at(i),std::vector<double>(x.begin()+i*13,x.begin()+(i+1)*13-1));
        Ep += ep;
    }
    return Ep;
}

void OutputTransformer::fill_energy(std::map<std::string,double>& out, const size_t i, const StateType& res) const
{
    const double Ec = compute_kinetic_energy(i, res);
    const double Ep = compute_potential_energy(i, res);
    out[std::string("Ec(")+bodies.at(i).name+")"] = Ec;
    out[std::string("Ep(")+bodies.at(i).name+")"] = Ep;
    out[std::string("Em(")+bodies.at(i).name+")"] = Ec+Ep;
}

std::map<std::string,double> OutputTransformer::operator()(const Res& res) const
{
    if (bodies.empty())
    {
        THROW(__PRETTY_FUNCTION__, OutputTransformerException, "No bodies defined");
    }
    std::map<std::string,double> out;
    out["t"] = res.t;
    update_kinematics(res.x);
    for (auto that_body = bodies.begin() ; that_body != bodies.end() ; ++that_body)
    {
        if (env.w.use_count())
        {
            env.w->update_surface_elevation(that_body->M,env.k,res.t);
            const std::vector<double> dz = env.w->get_relative_wave_height();
            that_body->intersector->update_intersection_with_free_surface(dz);
        }
    }
    for (auto that_position = input.position_output.begin() ; that_position != input.position_output.end() ; ++that_position)
    {
        fill(out, *that_position);
    }
    for (auto that_angle = input.angles_output.begin() ; that_angle != input.angles_output.end() ; ++that_angle)
    {
        fill(out, *that_angle);
    }
    for (size_t i = 0 ; i < bodies.size() ; ++i)
    {
        fill_energy(out, i, res.x);
    }

    return out;
}
