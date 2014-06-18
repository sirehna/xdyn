/*
 * StateMacros.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef STATEMACROS_HPP_
#define STATEMACROS_HPP_

#include <vector>

typedef std::vector<double> StateType;

#define NB_OF_STATES_PER_BODY (13)

#define _X(x,i) (&x[0] + i*13)
#define _Y(x,i) (&x[0] + i*13+1)
#define _Z(x,i) (&x[0] + i*13+2)
#define _U(x,i) (&x[0] + i*13+3)
#define _V(x,i) (&x[0] + i*13+4)
#define _W(x,i) (&x[0] + i*13+5)
#define _P(x,i) (&x[0] + i*13+6)
#define _Q(x,i) (&x[0] + i*13+7)
#define _R(x,i) (&x[0] + i*13+8)
#define _QR(x,i) (&x[0] + i*13+9)
#define _QI(x,i) (&x[0] + i*13+10)
#define _QJ(x,i) (&x[0] + i*13+11)
#define _QK(x,i) (&x[0] + i*13+12)


#endif /* STATEMACROS_HPP_ */
