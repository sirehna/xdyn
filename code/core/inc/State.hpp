#ifndef CORE_INC_STATE_HPP_
#define CORE_INC_STATE_HPP_

#include "History.hpp"
#include "AbstractStates.hpp"

class State : public AbstractStates<History>
{
    public :

        State(const AbstractStates<double>& vals,
              const double t0);

    private :
        State();
};

#endif /* CORE_INC_STATE_HPP_ */
