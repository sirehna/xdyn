/*
 * ManeuveringInternal.cpp
 *
 *  Created on: Jan 21, 2015
 *      Author: cady
 */


#include "ManeuveringInternal.hpp"

using namespace maneuvering;

Node::Node(const std::vector<NodePtr>& children_) : children(children_)
{
}

Node::~Node()
{
}

std::vector<NodePtr> Node::get_children() const
{
    return children;
}

Nullary::Nullary() : Node(std::vector<NodePtr>())
{
}

Binary::Binary(const NodePtr& lhs, const NodePtr& rhs) : Node({lhs,rhs})
{
}

Constant::Constant(const double val_) : val(val_)
{
}

Function Constant::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& , const double )
            {
                return val;
            };
}

Unary::Unary(const NodePtr operand) : Node({operand})
{
}

NodePtr Unary::get_operand() const
{
    return children.front();
}

Cos::Cos(const NodePtr& operand) : Unary(operand)
{
}
Function Cos::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return cos(op(states, ds, t));
            };
}

Sin::Sin(const NodePtr& operand) : Unary(operand)
{
}
Function Sin::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return sin(op(states, ds, t));
            };
}

Abs::Abs(const NodePtr& operand) : Unary(operand)
{
}

Function Abs::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::abs(op(states, ds, t));
            };
}

Log::Log(const NodePtr& operand) : Unary(operand)
{
}

Function Log::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::log(op(states, ds, t));
            };
}

Sum::Sum(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{

}

NodePtr Binary::get_lhs() const
{
    return children.at(0);
}

NodePtr Binary::get_rhs() const
{
    return children.at(1);
}

Function Sum::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) + op2(states, ds, t);
    };
}

NodePtr maneuvering::make_constant(const double val)
{
    return NodePtr(new Constant(val));
}

NodePtr maneuvering::make_cos(const NodePtr& n)
{
    return NodePtr(new Cos(n));
}

NodePtr maneuvering::make_sin(const NodePtr& n)
{
    return NodePtr(new Sin(n));
}

NodePtr maneuvering::make_abs(const NodePtr& n)
{
    return NodePtr(new Abs(n));
}

NodePtr maneuvering::make_log(const NodePtr& n)
{
    return NodePtr(new Log(n));
}

NodePtr maneuvering::make_sum(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Sum(lhs,rhs));
}
