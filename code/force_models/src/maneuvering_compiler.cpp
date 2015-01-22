/*
 * maneuvering_compiler.cpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#include "ManeuveringInternal.hpp"
#include "maneuvering_compiler.hpp"

using namespace maneuvering;

NodePtr compile_constant(const Op& c);
NodePtr compile_constant(const Op& c)
{
    return make_constant(static_cast<ConstantNode const*>(&c)->get_value());
}

NodePtr maneuvering::compile(const Op& ast)
{
    if (ast.name == "constant") return compile_constant(ast);
    return NodePtr();
}

