/*
 * EverythingObserver.cpp
 *
 *  Created on: Jul 9, 2019
 *      Author: cady
 */

#include "EverythingObserver.hpp"
#include "Sim.hpp"
#include "StateMacros.hpp"

EverythingObserver::EverythingObserver() : MapObserver({}), res()
{
}

std::string get_body_name(const Sim& sys);
std::string get_body_name(const Sim& sys)
{
    if (sys.get_bodies().empty())
    {
        return "";
    }
    return sys.get_bodies().front()->get_name();
}

void EverythingObserver::observe(const Sim& sys, const double t)
{
    observe_everything(sys, t);
    const std::string body_name = get_body_name(sys);
    for (const auto key_values:m)
    {
        const auto inserter = get_inserter(body_name, key_values.first);
        add(inserter, key_values.second);
    }
}

std::function<void(Res&, const double)> get_state_inserter(const size_t idx);
std::function<void(Res&, const double)> get_state_inserter(const size_t idx)
{
    return [idx](Res& res, const double value){res.x.resize(13);res.x[idx] = value;};
}

std::function<void(Res&, const double)> EverythingObserver::get_inserter(const std::string& body_name, const std::string& var_name) const
{
    if (var_name == std::string("t"))
    {
        return [](Res& res, const double value){res.t = value;};
    }
    if (var_name == std::string("x(") + body_name + ")")
    {
        return get_state_inserter(XIDX(0));
    }
    if (var_name == std::string("y(") + body_name + ")")
    {
        return get_state_inserter(YIDX(0));
    }
    if (var_name == std::string("z(") + body_name + ")")
    {
        return get_state_inserter(ZIDX(0));
    }
    if (var_name == std::string("u(") + body_name + ")")
    {
        return get_state_inserter(UIDX(0));
    }
    if (var_name == std::string("v(") + body_name + ")")
    {
        return get_state_inserter(VIDX(0));
    }
    if (var_name == std::string("w(") + body_name + ")")
    {
        return get_state_inserter(WIDX(0));
    }
    if (var_name == std::string("p(") + body_name + ")")
    {
        return get_state_inserter(PIDX(0));
    }
    if (var_name == std::string("q(") + body_name + ")")
    {
        return get_state_inserter(QIDX(0));
    }
    if (var_name == std::string("r(") + body_name + ")")
    {
        return get_state_inserter(RIDX(0));
    }
    if (var_name == std::string("qr(") + body_name + ")")
    {
        return get_state_inserter(QRIDX(0));
    }
    if (var_name == std::string("qi(") + body_name + ")")
    {
        return get_state_inserter(QIIDX(0));
    }
    if (var_name == std::string("qj(") + body_name + ")")
    {
        return get_state_inserter(QJIDX(0));
    }
    if (var_name == std::string("qk(") + body_name + ")")
    {
        return get_state_inserter(QKIDX(0));
    }
    return [var_name](Res& res, const double value){res.extra_observations[var_name] = value;};
}



void EverythingObserver::add(const std::function<void(Res&, const double)>& inserter, const std::vector<double>& values)
{
    if (values.empty())
    {
        return;
    }
    const size_t n = values.size();
    if (res.size() < n)
    {
        res.resize(n);
    }
    for (size_t i = 0 ; i < n ; ++i)
    {
        inserter(res[i], values[i]);
    }
}

std::vector<Res> EverythingObserver::get() const
{
    return res;
}
