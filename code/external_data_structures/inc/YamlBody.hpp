/*
 * YamlBody.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: cady
 */

#ifndef YAMLBODY_HPP_
#define YAMLBODY_HPP_

#include "YamlPosition.hpp"
#include "YamlSpeed.hpp"
#include "YamlDynamics.hpp"
#include "YamlModel.hpp"

#include <vector>
#include <string>

enum class InterpolationType {PIECEWISE_CONSTANT, LINEAR, SPLINE};
enum class BlockableState {U, V, W, P, Q, R};

template <typename T> struct YamlDOF
{
    YamlDOF() :
        state(BlockableState::U),
        t(),
        value(),
        interpolation(InterpolationType::PIECEWISE_CONSTANT)
    {}
    virtual ~YamlDOF() {}
    BlockableState state;
    T t;
    T value;
    InterpolationType interpolation;
};

struct YamlCSVDOF : public YamlDOF<std::string>
{
    YamlCSVDOF();
    std::string filename;
};

struct YamlBlockedDOF
{
    YamlBlockedDOF();
    std::vector<YamlDOF<std::vector<double> > > from_yaml;
    std::vector<YamlCSVDOF> from_csv;
};

struct YamlBody
{
    YamlBody();
    std::string name;
    std::string mesh;
    YamlPosition position_of_body_frame_relative_to_mesh;
    YamlPosition initial_position_of_body_frame_relative_to_NED_projected_in_NED;
    YamlSpeed initial_velocity_of_body_frame_relative_to_NED_projected_in_body;
    YamlDynamics dynamics;
    std::vector<YamlModel> external_forces;
    std::vector<YamlModel> controlled_forces;
    YamlBlockedDOF blocked_dof;
};

#endif /* YAMLBODY_HPP_ */
