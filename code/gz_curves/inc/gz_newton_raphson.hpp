/*
 * gz_newton_raphson.hpp
 *
 *  Created on: Feb 20, 2015
 *      Author: cady
 */

#ifndef GZ_NEWTON_RAPHSON_HPP_
#define GZ_NEWTON_RAPHSON_HPP_

#include <functional>

#include "GZTypes.hpp"

namespace GZ
{
    typedef std::function<Resultant(const ::GZ::State&)> FType;
}

#endif /* GZ_NEWTON_RAPHSON_HPP_ */
