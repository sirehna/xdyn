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
    COUT(children.size());
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

NodePtr Binary::get_lhs() const
{
    return children.at(0);
}

NodePtr Binary::get_rhs() const
{
    return children.at(1);
}

Sum::Sum(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
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

Pow::Pow(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Pow::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return std::pow(op1(states, ds, t), op2(states, ds, t));
    };
}

Exp::Exp(const NodePtr& operand) : Unary(operand)
{
}

Function Exp::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::exp(op(states, ds, t));
            };
}

Difference::Difference(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Difference::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) - op2(states, ds, t);
    };
}

Divide::Divide(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Divide::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) / op2(states, ds, t);
    };
}

Multiply::Multiply(const NodePtr& lhs_, const NodePtr& rhs_) : Binary(lhs_, rhs_)
{
}

Function Multiply::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
    {
        const auto op1 = get_lhs()->get_lambda();
        const auto op2 = get_rhs()->get_lambda();
        return op1(states, ds, t) * op2(states, ds, t);
    };
}

StateX::StateX(const NodePtr& operand) : Unary(operand)
{
}

Function StateX::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.x(op(states,ds,t));
            };
}

StateY::StateY(const NodePtr& operand) : Unary(operand)
{
}
Function StateY::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.y(op(states,ds,t));
            };
}

StateZ::StateZ(const NodePtr& operand) : Unary(operand)
{
}
Function StateZ::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.z(op(states,ds,t));
            };
}

StateU::StateU(const NodePtr& operand) : Unary(operand)
{
}
Function StateU::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.u(op(states,ds,t));
            };
}

StateV::StateV(const NodePtr& operand) : Unary(operand)
{
}
Function StateV::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.v(op(states,ds,t));
            };
}

StateW::StateW(const NodePtr& operand) : Unary(operand)
{
}
Function StateW::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.w(op(states,ds,t));
            };
}

StateP::StateP(const NodePtr& operand) : Unary(operand)
{
}
Function StateP::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.p(op(states,ds,t));
            };
}

StateQ::StateQ(const NodePtr& operand) : Unary(operand)
{
}
Function StateQ::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.q(op(states,ds,t));
            };
}

StateR::StateR(const NodePtr& operand) : Unary(operand)
{
}
Function StateR::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return states.r(op(states,ds,t));
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

NodePtr maneuvering::make_pow(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Pow(lhs,rhs));
}

NodePtr maneuvering::make_exp(const NodePtr& n)
{
    return NodePtr(new Exp(n));
}

NodePtr maneuvering::make_difference(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Difference(lhs,rhs));
}

NodePtr maneuvering::make_divide(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Divide(lhs,rhs));
}

NodePtr maneuvering::make_multiply(const NodePtr& lhs, const NodePtr& rhs)
{
    return NodePtr(new Multiply(lhs,rhs));
}

NodePtr maneuvering::make_state_x(const NodePtr& n)
{
    return NodePtr(new StateX(n));
}

NodePtr maneuvering::make_state_y(const NodePtr& n)
{
    return NodePtr(new StateY(n));
}

NodePtr maneuvering::make_state_z(const NodePtr& n)
{
    return NodePtr(new StateZ(n));
}

NodePtr maneuvering::make_state_u(const NodePtr& n)
{
    return NodePtr(new StateU(n));
}

NodePtr maneuvering::make_state_v(const NodePtr& n)
{
    return NodePtr(new StateV(n));
}

NodePtr maneuvering::make_state_w(const NodePtr& n)
{
    return NodePtr(new StateW(n));
}

NodePtr maneuvering::make_state_p(const NodePtr& n)
{
    return NodePtr(new StateP(n));
}

NodePtr maneuvering::make_state_q(const NodePtr& n)
{
    return NodePtr(new StateQ(n));
}

NodePtr maneuvering::make_state_r(const NodePtr& n)
{
    return NodePtr(new StateR(n));
}
