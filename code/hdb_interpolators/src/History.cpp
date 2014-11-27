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
    if (tau<0)
    {
        std::stringstream ss;
        ss << "Requesting value in the future: asked for t-tau with tau = " << tau;
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
    if (L.empty())
    {
        THROW(__PRETTY_FUNCTION__, HistoryException, "Cannot retrieve anything from history because it is empty");
    }
    const double t = get_current_time();
    const auto idx = find_braketing_position(t-tau);
    return get_value(idx, t-tau);
}

double History::get_current_time() const
{
    return L.back().first;
}

double History::get_value(const size_t idx, const double t) const
{
    if (idx == 0)
    {
        return L[idx].second;
    }
    if (idx == L.size())
    {
        THROW(__PRETTY_FUNCTION__, HistoryException, "Something is very wrong: first bracketing value is last value in list.");
    }
    const double tA = L[idx-1].first;
    const double tB = L[idx].first;
    const double yA = L[idx-1].second;
    const double yB = L[idx].second;

    return (t-tA)/(tB-tA)*(yB-yA) + yA;
}

void History::throw_if_already_added(const size_t idx, const double t) const
{
    if ((idx != L.size()) and (L[idx].first == t))
    {
        std::stringstream ss;
        ss << "Attempting to insert the same instant in History: t = " << t << " already exists.";
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
}

size_t History::find_braketing_position(const double t) const
{
    if (L.empty())            return L.size();
    if (L.back().first < t)   return L.size();
    if (L.front().first >= t) return 0;
    size_t idx_lower = 0;
    size_t idx_greater = L.size()-1;
    while (true)
    {
        if (t==L[idx_lower].first) return idx_lower;
        if (t==L[idx_greater].first) return idx_greater;
        if (idx_greater<=idx_lower+1) return idx_greater;
        const size_t idx_middle = (size_t)std::floor(((double)idx_lower+(double)idx_greater)/2.);
        const auto middle = L[idx_middle];
        if (t==middle.first) return idx_middle;
        if (t < middle.first)
        {
            idx_greater = idx_middle;
        }
        else // t > middle->first
        {
            idx_lower = idx_middle;
        }
    }
    return L.size();
}

void History::record(const double t, //!< Instant corresponding to the value being added
                     const double val //!< Value to add
                    )
{
    const size_t t_idx = find_braketing_position(t);
    throw_if_already_added(t_idx, t);
    L.insert(L.begin()+(long)t_idx,std::make_pair(t,val));
}

size_t History::size() const
{
    return L.size();
}
