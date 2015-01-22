/*
 * ManeuveringAST.cpp
 *
 *  Created on: Jan 20, 2015
 *      Author: cady
 */


#include "ManeuveringAST.hpp"

using namespace maneuvering;

Op::Op(const std::string& name_, const std::vector<Op>& sons_) :
       name(name_),
       sons(sons_)
{
}

Op::~Op()
{
}

VarNode::VarNode(const std::string& name_) : Op(name_, std::vector<Op>())
{
}

BinaryNode::BinaryNode(const std::string& operator_name, const Op& lhs, const Op& rhs) :
        Op(operator_name, {lhs, rhs})
{
}

UnaryNode::UnaryNode(const std::string& operator_name, const Op& son) :
        Op(operator_name, {son})
{
}

StateNode::StateNode(const std::string& name_, const Op& operand) :
        UnaryNode(name_, operand)
{
}

NullaryNode::NullaryNode(const std::string& operator_name) : Op(operator_name, {})
{
}

ConstantNode::ConstantNode(const double value_) : NullaryNode("constant"), value(value_)
{
}

double ConstantNode::get_value() const
{
    return value;
}
