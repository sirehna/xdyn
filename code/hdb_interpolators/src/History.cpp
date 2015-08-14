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

History::History(const double Tmax_) : Tmax(Tmax_), L(), oldest_recorded_instant(0)
{
}
#include <ssc/macros.hpp>
double History::operator()(double tau //!< How far back in history do we need to go (in seconds)?
                          ) const
{
    const double eps = 1E-12;
    if (std::abs(tau-Tmax)<eps) tau = Tmax;
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
    return get_value(tau);
}

double History::get_current_time() const
{
    return L.empty() ? oldest_recorded_instant : L.back().first;
}

double History::get_value(const double tau) const
{
    const double t = get_current_time();
    const auto idx = find_braketing_position(t-tau);
    return interpolate_value_in_interval(idx, t-tau);
}

double History::interpolate_value_in_interval(const size_t idx, const double t) const
{
    if ((idx == 0) or (idx >= L.size()))
    {
        return L[0].second;
    }
    const double tA = L[idx-1].first;
    const double tB = L[idx].first;
    const double yA = L[idx-1].second;
    const double yB = L[idx].second;

    if (std::abs(t-tA) < 1E-12) return yA; // To fix bug 2655
    if (std::abs(t-tB) < 1E-12) return yB;
                                return (t-tA)/(tB-tA)*(yB-yA) + yA;
}

void History::throw_if_already_added(const size_t idx, const double t, const double val) const
{
    if ((idx != L.size()) and (L[idx].first == t) and (val != L[idx].second))
    {
        std::stringstream ss;
        ss << "Attempting to insert the same instant in History with different value: t = " << t << " already exists.";
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
}

size_t History::find_braketing_position(const double t) const
{
    if (L.empty())                    return 0;
    if (L.back().first < t)           return L.size();
    if (L.front().first >= t)         return 0;
    size_t idx_lower = 0;
    size_t idx_greater = L.size()-1;
    while (true)
    {
        if (t==L[idx_lower].first)    return idx_lower;
        if (t==L[idx_greater].first)  return idx_greater;
        if (idx_greater<=idx_lower+1) return idx_greater;
        const size_t idx_middle = (size_t)std::floor(((double)idx_lower+(double)idx_greater)/2.);
        const auto middle = L[idx_middle];
        if (t==middle.first)          return idx_middle;
        if (t < middle.first)
        {
            idx_greater = idx_middle;
        }
        else // t > middle.first
        {
            idx_lower = idx_middle;
        }
    }
                                      return L.size();
}

void History::shift_oldest_recorded_instant_if_necessary()
{
    if (get_current_time() - oldest_recorded_instant >= Tmax)
    {
        oldest_recorded_instant = get_current_time()-Tmax;
        const double vmin = interpolate_value_in_interval(1, oldest_recorded_instant);
        const size_t idx = find_braketing_position(oldest_recorded_instant);
        L.erase(L.begin(), L.begin() + (long) (idx));
        if (L.front().first != oldest_recorded_instant)
            L.insert(L.begin(), std::make_pair(oldest_recorded_instant, vmin));
    }
}

void History::add_value_to_history(const double t, const double val)
{
    const size_t idx = find_braketing_position(t);
    if ((idx != L.size()) and (L[idx].first == t))
    {
        L[idx] = std::make_pair(t, val);
    }
    else
    {
        L.insert(L.begin() + (long) (idx), std::make_pair(t, val));
    }
}

void History::update_oldest_recorded_instant(const double t)
{
    if (L.empty()) oldest_recorded_instant = t;
    oldest_recorded_instant = std::min(oldest_recorded_instant, t);
}

void History::record(const double t, //!< Instant corresponding to the value being added
                     const double val //!< Value to add
                    )
{
    update_oldest_recorded_instant(t);
    add_value_to_history(t, val);
    shift_oldest_recorded_instant_if_necessary();
}

size_t History::size() const
{
    return L.size();
}

double History::get_Tmax() const
{
    return Tmax;
}

double History::get_duration() const
{
    return L.back().first - L.front().first;
}

std::ostream& operator<<(std::ostream& os, const History& h)
{
    os << "[";
    for (size_t i = 0 ; i < h.L.size() -1 ; ++i)
    {
        os << "(" << h.L.at(i).first << "," << h.L.at(i).second << "), ";
    }
    if (not(h.L.empty())) os << "(" << h.L.back().first << "," << h.L.back().second << ")";
    os << "]";
    return os;
}

double History::average(const double T) const
{
    if (L.size() < 2)
    {
        THROW(__PRETTY_FUNCTION__, HistoryException, "Cannot retrieve average value because history it contains less than two elements");
    }
    if (T > get_duration())
    {
        std::stringstream ss;
        ss << "Cannot retrieve average value because history is not long enough: it currently has a duration of " << get_duration() << " s, but the average was requested on " << T << " s";
        THROW(__PRETTY_FUNCTION__, HistoryException, ss.str());
    }
    return L.front().second;
}
