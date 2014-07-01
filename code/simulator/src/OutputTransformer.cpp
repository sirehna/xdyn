/*
 * OutputGenerator.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputTransformer.hpp"
#include "Kinematics.hpp"
#include "update_kinematics.hpp"
#include "SimulatorBuilder.hpp"
#include "Transform.hpp"
#include "OutputTransformerException.hpp"

OutputTransformer::OutputTransformer(const SimulatorBuilder& builder) : input(builder.get_parsed_yaml()), bodies(std::vector<Body>()), points(std::map<std::string,Point>()), k(TR1(shared_ptr)<Kinematics>(new Kinematics()))
{
    MeshMap m;
    for (const auto body:input.bodies)
    {
        m[body.name] = VectorOfVectorOfPoints();
    }
    bodies = builder.get_bodies(m);
    for (const auto point:input.points) points[point.name] = Point(point.frame,point.x,point.y,point.z);
}

void OutputTransformer::update_kinematics(const StateType& x) const
{
    for (size_t i = 0 ; i < bodies.size() ; ++i) ::update_kinematics(x, bodies[i], i, k);
}

template <typename T> double get_axis_value(const T& P, const std::string& axis)
{
    if (axis=="x") return P.x();
    if (axis=="y") return P.y();
    if (axis=="z") return P.z();
                   return 0;
}

void OutputTransformer::fill(std::map<std::string,double>& out, const YamlPositionOutput& position) const
{
    const auto that_point = points.find(position.point);
    if (that_point != points.end())
    {
        const Point O1P = that_point->second;
        const auto b1 = O1P.get_frame();
        const auto b2 = position.relative_to_frame;
        const auto b3 = position.projected_in_frame;
        const kinematics::Transform T13 = k->get(b1, b3);
        const auto T12 = k->get(b1, b2);
        const Point O1O2 = T12.get_point();
        const Point P = Point(b3,T13.get_rot()*(O1P - O1O2));
        for (auto axis:position.axes)
        {
            const auto varname = axis + "(" + that_point->first + " in " + b1 + " / " + b2 + " -> " + b3 + ")";
            out[varname] = get_axis_value(P, axis);
        }
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, OutputTransformerException, std::string("Unable to find point '") + position.point + "' in input data");
    }
}

std::map<std::string,double> OutputTransformer::operator()(const Res& res) const
{
    std::map<std::string,double> out;
    out["t"] = res.t;
    update_kinematics(res.x);
    for (const auto position:input.position_output) fill(out, position);

    return out;
}
