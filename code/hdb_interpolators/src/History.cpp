/*
 * History.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#include <sstream>

#include "History.hpp"
#include "HistoryException.hpp"



History::History(const double Tmax_) : Tmax(Tmax_)
{
    if (Tmax<=0)
    {
        std::stringstream ss;
        ss << "Tmax should be greater than 0: received " << Tmax;
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
}

double History::get(double tau //!< How far back in history do we need to go (in seconds)?
                               ) const
{
    if (tau>Tmax)
    {
        std::stringstream ss;
        ss << "Requesting value too far in the past: asked for t-" << tau << ", but history only goes back to t-" << Tmax;
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
    return tau;
}
