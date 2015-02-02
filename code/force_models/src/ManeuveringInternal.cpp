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

void Constant::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Cos::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Sin::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Abs::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Log::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Sum::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Pow::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Exp::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Function Sqrt::get_lambda() const
{
    return [this](const BodyStates& states, ssc::data_source::DataSource& ds, const double t)
            {
                const auto op = get_operand()->get_lambda();
                return std::sqrt(op(states, ds, t));
            };
}

Sqrt::Sqrt(const NodePtr& operand) : Unary(operand)
{
}

void Sqrt::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Difference::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Divide::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
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

void Multiply::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

Time::Time()
{
}

Function Time::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& , const double t)
            {
                return t;
            };
}

void Time::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

UnknownIdentifier::UnknownIdentifier(const std::string& identifier_name_) : identifier_name(identifier_name_)
{
}

Function UnknownIdentifier::get_lambda() const
{
    return [this](const BodyStates& , ssc::data_source::DataSource& ds, const double )
            {
                return ds.get<double>(identifier_name);
            };
}

void UnknownIdentifier::accept(AbstractNodeVisitor& visitor) const
{
    visitor.visit(*this);
}

std::string UnknownIdentifier::get_name() const
{
    return identifier_name;
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

NodePtr maneuvering::make_sqrt(const NodePtr& n)
{
    return NodePtr(new Sqrt(n));
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
    return NodePtr(new State<StateType::X>(n));
}

NodePtr maneuvering::make_state_y(const NodePtr& n)
{
    return NodePtr(new State<StateType::Y>(n));
}

NodePtr maneuvering::make_state_z(const NodePtr& n)
{
    return NodePtr(new State<StateType::Z>(n));
}

NodePtr maneuvering::make_state_u(const NodePtr& n)
{
    return NodePtr(new State<StateType::U>(n));
}

NodePtr maneuvering::make_state_v(const NodePtr& n)
{
    return NodePtr(new State<StateType::V>(n));
}

NodePtr maneuvering::make_state_w(const NodePtr& n)
{
    return NodePtr(new State<StateType::W>(n));
}

NodePtr maneuvering::make_state_p(const NodePtr& n)
{
    return NodePtr(new State<StateType::P>(n));
}

NodePtr maneuvering::make_state_q(const NodePtr& n)
{
    return NodePtr(new State<StateType::Q>(n));
}

NodePtr maneuvering::make_state_r(const NodePtr& n)
{
    return NodePtr(new State<StateType::R>(n));
}

NodePtr maneuvering::make_time()
{
    return NodePtr(new Time());
}

NodePtr maneuvering::make_unknown_identifier(const std::string& identifier_name)
{
    return NodePtr(new UnknownIdentifier(identifier_name));
}

namespace maneuvering
{
    template <> void State<StateType::X>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::Y>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::Z>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::U>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::V>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::W>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::P>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::Q>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
    template <> void State<StateType::R>::accept(AbstractNodeVisitor& visitor) const
    {
        visitor.visit(*this);
    }
}