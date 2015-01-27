/*
 * maneuvering_compiler.hpp
 *
 *  Created on: Jan 22, 2015
 *      Author: cady
 */


#ifndef MANEUVERING_COMPILER_HPP_
#define MANEUVERING_COMPILER_HPP_

#include "ManeuveringInternal.hpp"

namespace maneuvering
{
    NodePtr compile(const std::string& expression);
}


#endif  /* MANEUVERING_COMPILER_HPP_ */
