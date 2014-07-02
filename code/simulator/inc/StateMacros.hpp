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

#define XIDX(i) (i*13)
#define YIDX(i) (i*13+1)
#define ZIDX(i) (i*13+2)
#define UIDX(i) (i*13+3)
#define VIDX(i) (i*13+4)
#define WIDX(i) (i*13+5)
#define PIDX(i) (i*13+6)
#define QIDX(i) (i*13+7)
#define RIDX(i) (i*13+8)
#define QRIDX(i) (i*13+9)
#define QIIDX(i) (i*13+10)
#define QJIDX(i) (i*13+11)
#define QKIDX(i) (i*13+12)

#define _X(x,i) (&x[0] + XIDX(i))
#define _Y(x,i) (&x[0] + YIDX(i))
#define _Z(x,i) (&x[0] + ZIDX(i))
#define _U(x,i) (&x[0] + UIDX(i))
#define _V(x,i) (&x[0] + VIDX(i))
#define _W(x,i) (&x[0] + WIDX(i))
#define _P(x,i) (&x[0] + PIDX(i))
#define _Q(x,i) (&x[0] + QIDX(i))
#define _R(x,i) (&x[0] + RIDX(i))
#define _QR(x,i) (&x[0] + QRIDX(i))
#define _QI(x,i) (&x[0] + QIIDX(i))
#define _QJ(x,i) (&x[0] + QJIDX(i))
#define _QK(x,i) (&x[0] + QKIDX(i))


#endif /* STATEMACROS_HPP_ */
