/*
 * maneuvering_compiler.hpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#ifndef MANEUVERING_COMPILER_HPP_
#define MANEUVERING_COMPILER_HPP_

#include "ManeuveringAST.hpp"
#include "ManeuveringInternal.hpp"

namespace maneuvering
{
    NodePtr compile(const Op& ast);
}


#endif  /* MANEUVERING_COMPILER_HPP_ */
