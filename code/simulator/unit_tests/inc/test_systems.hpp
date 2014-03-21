/*
 * test_systems.hpp
 *
 *  Created on: 21 mars 2014
 *      Author: maroff
 */

#ifndef TEST_SYSTEMS_HPP_
#define TEST_SYSTEMS_HPP_

typedef std::vector<double> StateType;

struct Pendulum
{
    Pendulum(const double mu, const double omega, const double eps, StateType& x) : m_mu(mu),m_omega(omega),m_eps(eps),state(x)
    {
    }

    void operator()(const StateType &x, StateType &dxdt, double t) const
    {
        dxdt[0] = x[1];
        dxdt[1] = -sin(x[0]) - m_mu*x[1] + m_eps*sin(m_omega*t);
    }

    double m_mu, m_omega, m_eps;
    StateType& state;
};

struct SimpleODE
{
    SimpleODE(const StateType& x0) : state(x0)
    {
    }

    void operator()(const StateType &x, StateType &dxdt, double )
    {
        state = x;
        dxdt[0] = 2;//*t;//2*x[0];
    }

    void update_discrete_states()
    {
    }

    void update_continuous_states()
    {
    }

    StateType state;
};

struct QuadraticODE
{
    QuadraticODE(const StateType& x0) : state(x0)
    {
    }

    void operator()(const StateType &x, StateType &dxdt, double t)
    {
        state = x;
        dxdt[0] = 2*t;
    }

    StateType state;
};



#endif /* TEST_SYSTEMS_HPP_ */
