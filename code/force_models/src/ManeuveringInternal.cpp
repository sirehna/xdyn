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
