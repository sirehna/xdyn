/*
 * History.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#include <cmath>
#include <iterator>
#include <sstream>

#include "History.hpp"
#include "HistoryException.hpp"



History::History(const double Tmax_) : Tmax(Tmax_), L()
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

void History::throw_if_already_added(const Container::const_iterator& it, const double t) const
{
    if ((it != L.end()) and (it->first == t))
    {
        std::stringstream ss;
        ss << "Attempting to insert the same instant in History: t = " << t << " already exists.";
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
}

History::Container::const_iterator History::find_braketing_position(const double t) const
{
    if (L.empty())     return L.end();
    if (L.back().first < t)  return L.end();
    if (L.front().first > t) return L.begin();
    size_t idx_lower = 0;
    size_t idx_greater = L.size()-1;
    while (true)
    {
        const auto lower = L.begin()+(long)idx_lower;
        const auto greater = L.begin()+(long)idx_greater;
        if (t==lower->first) return lower;
        if (t==greater->first) return greater;
        if (idx_greater==idx_lower+1) return L.begin()+(long)idx_lower;
        const size_t idx_middle = (size_t)std::floor(((double)idx_lower+(double)idx_greater)/2.);
        const auto middle = L.begin()+(long)idx_middle;
        if (t==middle->first) return middle;
        if (t < middle->first)
        {
            idx_greater = idx_middle;
        }
        else // t > middle->first
        {
            idx_lower = idx_middle;
        }
    }
    return L.end();
}

void History::record(const double t, //!< Instant corresponding to the value being added
                     const double val //!< Value to add
                    )
{
    Container::const_iterator it = find_braketing_position(t);
    throw_if_already_added(it, t);
    const long d = it-L.begin();
    L.insert(L.begin()+d,std::make_pair(t,val));
}
