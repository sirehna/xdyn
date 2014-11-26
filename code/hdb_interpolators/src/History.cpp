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

void History::record(const double t, //!< Instant corresponding to the value being added
                     const double val //!< Value to add
                    )
{
    if (L.empty())
    {
        L.push_back(std::make_pair(t, val));
    }
    else
    {
        bool previous_is_lower = (L.front().first < t);
        if (L.size() == 1)
        {
            if (L.front().first == t)
            {
                std::stringstream ss;
                ss << "Attempting to insert the same instant in History: t = " << t << " already exists.";
                THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
            }
            if (previous_is_lower) L.push_back(std::make_pair(t, val));
            else                   L.push_front(std::make_pair(t, val));
        }
        else
        {
            auto it = L.begin();
            if (it->first == t)
            {
                std::stringstream ss;
                ss << "Attempting to insert the same instant in History: t = " << t << " already exists.";
                THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
            }
            it++;
            for ( ; it != L.end() ; ++it)
            {
                if (it->first == t)
                {
                    std::stringstream ss;
                    ss << "Attempting to insert the same instant in History: t = " << t << " already exists.";
                    THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
                }
                bool next_is_greater = it->first > t;
                if (previous_is_lower and next_is_greater) L.insert(it, std::make_pair(t, val));
                previous_is_lower = not(next_is_greater);
            }
            if (previous_is_lower) L.push_back(std::make_pair(t, val));
        }
    }
}
