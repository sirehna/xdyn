/*
 * DsSolve.cpp
 *
 *  Created on: 25 mars 2014
 *      Author: maroff
 */

#include <boost/numeric/odeint/stepper/euler.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta_cash_karp54.hpp>

#include "DsSolve.hpp"
#include "solve.hpp"
#include "DsSystemMacros.hpp"



typedef std::vector<double> StateType;
typedef boost::numeric::odeint::euler<StateType> EulerStepper;
typedef boost::numeric::odeint::runge_kutta4<StateType> RK4Stepper;
typedef boost::numeric::odeint::runge_kutta_cash_karp54<StateType> RKCK;

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


void integrate(DataSource& ds, const double tstart, const double tend, DefaultObserver<DSSystem>& observer)
{
    DSSystem sys(ds);
    std::vector<double> x0(2,0);
    double dt = GET(ds, simulator_base::initial_time_step);
    const solver::SolverType solver_type = GET(ds, simulator_base::stepper);
    switch (solver_type)
    {
        case solver::EULER:
            quicksolve<EulerStepper>(sys, tstart, tend, dt, observer);
            break;
        case solver::RK4:
            quicksolve<RK4Stepper>(sys, tstart, tend, dt, observer);
            break;
        case solver::RKCK54:
            quicksolve<RKCK>(sys, tstart, tend, dt, observer);
            break;
        default:
            break;
    }
}
