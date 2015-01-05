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

#define XIDX(i) (i*NB_OF_STATES_PER_BODY)
#define YIDX(i) (i*NB_OF_STATES_PER_BODY+1)
#define ZIDX(i) (i*NB_OF_STATES_PER_BODY+2)
#define UIDX(i) (i*NB_OF_STATES_PER_BODY+3)
#define VIDX(i) (i*NB_OF_STATES_PER_BODY+4)
#define WIDX(i) (i*NB_OF_STATES_PER_BODY+5)
#define PIDX(i) (i*NB_OF_STATES_PER_BODY+6)
#define QIDX(i) (i*NB_OF_STATES_PER_BODY+7)
#define RIDX(i) (i*NB_OF_STATES_PER_BODY+8)
#define QRIDX(i) (i*NB_OF_STATES_PER_BODY+9)
#define QIIDX(i) (i*NB_OF_STATES_PER_BODY+10)
#define QJIDX(i) (i*NB_OF_STATES_PER_BODY+11)
#define QKIDX(i) (i*NB_OF_STATES_PER_BODY+12)

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
