/*
 * History.cpp
 *
 *  Created on: Nov 26, 2014
 *      Author: cady
 */

#include "History.hpp"
#include "InternalErrorException.hpp"
#include <cmath>
#include <cstdint>
#include <iterator>
#include <sstream>

union Double
{
    Double(const double num) : f(num) {}
    // Portable extraction of components.
    bool negative() const { return i < 0; }
    int64_t i;
    double f;
};

bool almost_equal(const double a, const double b, const int maxUlpsDiff = 4);
bool almost_equal(const double a, const double b, const int maxUlpsDiff)
{
    const Double uA(a);
    const Double uB(b);

    // Different signs means they do not match.
    if (uA.negative() != uB.negative())
    {
        // Check for equality to make sure +0==-0
        if (a == b)
            return true;
        return false;
    }

    // Find the difference in ULPs.
    if ( std::abs(uA.i - uB.i) <= maxUlpsDiff)
        return true;

    return false;
}

History::History(const double Tmax_) : Tmax(Tmax_), L(), oldest_recorded_instant(0)
{
}

double get_tmax(const std::vector<std::pair<double,double> >& L);
double get_tmax(const std::vector<std::pair<double,double> >& L)
{
    if (L.empty()) return 0;
    return L.back().first - L.front().first;
}

History::History(const Container& L_) : Tmax(get_tmax(L_)), L(L_), oldest_recorded_instant(L.empty()?0:L.front().first)
{
}

double History::operator()(double tau //!< How far back in history do we need to go (in seconds)?
                          ) const
{
    const double eps = 1E-12;
    if (std::abs(tau-Tmax)<eps) tau = Tmax;
    if (tau>Tmax)
    {
        return 0;
    }
    if (tau<0)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
                "Requesting value in the future: asked for t-tau with tau = " << tau);
    }
    if (L.empty())
    {
        return 0;
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

    if (std::abs(t-tA) < 1E-12)
    {
        return yA; // To fix bug 2655
    }
    if (std::abs(t-tB) < 1E-12)
    {
        return yB;
    }
    return (t-tA)/(tB-tA)*(yB-yA) + yA;
}

void History::throw_if_already_added(const size_t idx, const double t, const double val) const
{
    if ((idx != L.size()) and (L[idx].first == t) and (val != L[idx].second))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
                "Attempting to insert the same instant in History with different value: t = " << t << " already exists.");
    }
}

size_t History::find_braketing_position(const double t) const
{
    if (L.empty())return 0;
    if (L.back().first < t)           return L.size();
    if (L.front().first >= t)         return 0;
    size_t idx_lower = 0;
    size_t idx_greater = L.size()-1;
    while (true)
    {
        if (t==L[idx_lower].first)
        {
            return idx_lower;
        }
        if (t==L[idx_greater].first)
        {
            return idx_greater;
        }
        if (idx_greater<=idx_lower+1)
        {
            return idx_greater;
        }
        const size_t idx_middle = (size_t)std::floor(((double)idx_lower+(double)idx_greater)/2.);
        const auto middle = L[idx_middle];
        if (t==middle.first)
        {
            return idx_middle;
        }
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
        if (not(almost_equal(L.front().first, oldest_recorded_instant,32)))
        {
            L.insert(L.begin(), std::make_pair(oldest_recorded_instant, vmin));
        }
    }
}

void History::add_value_to_history(const double t, const double val)
{
    const size_t idx = find_braketing_position(t);
    if ((idx != L.size()) and (almost_equal(L[idx].first, t)))
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

void History::record(double t, //!< Instant corresponding to the value being added
                     const double val //!< Value to add
                    )
{
    if (not(L.empty()))
    {
        if  (almost_equal(t,L.back().first))
        {
            t = L.back().first;
        }
        if (t < L.back().first)
        {
            THROW(__PRETTY_FUNCTION__
                 , InternalErrorException
                 , "history must be recorded in a strictly increasing order: trying to add t = "
                   << t
                   <<
                   ", but the latest timestamp in history is "
                   << L.back().first
                   << " (t-thistory.back = "
                    << t-L.back().first
                    << ")");
        }
    }
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
    if (L.empty()) return 0;
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

double History::trapeze(const double xa, const double ya, const double xb, const double yb) const
{
    return (xb-xa)*(ya+yb)/2.;
}

double History::integrate(const size_t idx) const
{
    double ret = 0;
    for (size_t i = idx ; i < L.size()-1 ; ++i)
    {
        ret += trapeze(L.at(i).first, L.at(i).second, L.at(i+1).first, L.at(i+1).second);
    }
    return ret;
}

void History::check_if_average_can_be_retrieved(const double T) const
{
    if (T < 0)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException,
                "Cannot retrieve average value because supplied length is negative: received T = "
                << T);
    }
}

double History::average(double T) const
{
    if (L.empty()) return 0;
    if (L.size()==1) return L.front().second;
    check_if_average_can_be_retrieved(T);
    T = std::min(T, get_duration());
    const double t = get_current_time() - T;
    const size_t idx = find_braketing_position(t);
    const double first_value = interpolate_value_in_interval(idx, t);
    const double integral_of_first_interval = trapeze(t, first_value, L.at(idx).first, L.at(idx).second);
    const double integral_from_t_to_now = integrate(idx);
    return  (T!=0) ? (integral_of_first_interval + integral_from_t_to_now)/T : L.back().second;
}

std::pair<double,double> History::operator[](const int index) const
{
    if(index>=0)
    {
        return L[(size_t)index];
    }
    else
    {
        return L[L.size()+(size_t)index];
    }
}

void History::reset()
{
    L.clear();
    oldest_recorded_instant = 0;
}

bool History::is_empty() const
{
    return L.size() == 0;
}

std::vector<double> History::get_values(const double tmax) const
{
    if (tmax == 0)
    {
        return {this->operator()(0)};
    }
    std::vector<double> ret;
    ret.reserve(L.size());
    const double t = get_current_time();
    for (size_t i = 0 ; i < L.size() ; ++i)
    {
        if (tmax >= t - L[i].first)
        {
            ret.push_back(L[i].second);
        }
    }
    return ret;
}

std::vector<double> History::get_dates(const double tmax) const
{
    const double t = get_current_time();
    if (tmax == 0)
    {
        return {t};
    }
    std::vector<double> ret;
    ret.reserve(L.size());

    for (size_t i = 0 ; i < L.size() ; ++i)
    {
        if (tmax >= t - L[i].first)
        {
            ret.push_back(L[i].first);
        }
    }
    return ret;
}
