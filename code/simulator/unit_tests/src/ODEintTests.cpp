/*
 * ODEintTests.cpp
 *
 *  Created on: 7 mars 2014
 *      Author: maroff
 */

#include "ODEintTests.hpp"

#include "Observers.hpp"
#include "DefaultScheduler.hpp"
#include "detection_of_state_update_methods.hpp"
#include "solve.hpp"
#include "test_systems.hpp"

#include <boost/numeric/odeint/stepper/euler.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta4.hpp>
#include <boost/numeric/odeint/stepper/runge_kutta_cash_karp54.hpp>

#include <sstream>

ODEintTests::ODEintTests() : a(DataGenerator(452251321))
{
}

ODEintTests::~ODEintTests()
{
}

void ODEintTests::SetUp()
{
}

void ODEintTests::TearDown()
{
}

typedef boost::numeric::odeint::euler<StateType> EulerStepper;
typedef boost::numeric::odeint::runge_kutta4<StateType> RK4Stepper;
typedef boost::numeric::odeint::runge_kutta_cash_karp54<StateType> RKCK;

TEST_F(ODEintTests, linear)
{
//! [ODEintTests example]
    StateType x0(2,0);
    SimpleODE system(x0);
    double t0 = 0.0 , tend = 10.0 , dt = 1;

    VectorObserver<SimpleODE> observer;
    quicksolve<EulerStepper>(system, t0, tend, dt, observer);
//! [ODEintTests example]

//! [ODEintTests expected output]
    const std::vector<std::pair<double,double> > v = observer.get();
    ASSERT_EQ(11,v.size());
    for (size_t i = 0 ; i <= 10 ; ++i)
    {
        ASSERT_EQ(i, v.at(i).first);
        ASSERT_DOUBLE_EQ(2*i, v.at(i).second);
    }
//! [ODEintTests expected output]
}

TEST_F(ODEintTests, linear_with_adaptive_RK_Cash_Karp)
{
    StateType x0(2,0);
    SimpleODE system(x0);
    double t0 = 0.0 , tend = 10.0 , dt = 1;

    VectorObserver<SimpleODE> observer;
    quicksolve<RKCK>(system, t0, tend, dt, observer);

    const std::vector<std::pair<double,double> > v = observer.get();
    ASSERT_EQ(11,v.size());
    for (size_t i = 0 ; i <= 10 ; ++i)
    {
        ASSERT_EQ(i, v.at(i).first);
        ASSERT_DOUBLE_EQ(2*i, v.at(i).second);
    }
}

TEST_F(ODEintTests, quadratic_with_euler)
{
    StateType x0(2,0);
    QuadraticODE system(x0);
    const size_t N = 10;
    double t0 = 0.0 , tend = N , dt = 1;

    VectorObserver<QuadraticODE> observer;
    quicksolve<EulerStepper>(system, t0, tend, dt, observer);

    const std::vector<std::pair<double,double> > v = observer.get();
    ASSERT_EQ(N+1,v.size());
    for (size_t i = 0 ; i <= N ; ++i)
    {
        ASSERT_EQ(i, v.at(i).first);
        ASSERT_DOUBLE_EQ(i*(i-1), v.at(i).second);
    }
}

TEST_F(ODEintTests, quadratic_with_RK4)
{
    StateType x0(2,0);
    QuadraticODE system(x0);
    const size_t N = 10;
    double t0 = 0.0 , tend = N , dt = 1;

    VectorObserver<QuadraticODE> observer;
    quicksolve<RK4Stepper>(system, t0, tend, dt, observer);

    const std::vector<std::pair<double,double> > v = observer.get();
    ASSERT_EQ(N+1,v.size());
    for (size_t i = 0 ; i <= N ; ++i)
    {
        ASSERT_EQ(i, v.at(i).first);
        ASSERT_DOUBLE_EQ(i*i, v.at(i).second);
    }
}

