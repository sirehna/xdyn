/*
 * dynamho_tests.cpp
 *
 *  Created on: 24 févr. 2014
 *      Author: maroff
 */

#include "dynamho_tests.hpp"
#include "generate_yaml_data.hpp"
#include "DynamhoYamlParser.hpp"
#include "DynamhoSimulation.hpp"
#include "DataSource.hpp"
#include "DataSourceModule.hpp"
#include "hydrodynamic.hpp"
#include "hydrostatic.hpp"
#include "inertia_coupling.hpp"
#include "propulsion.hpp"
#include "test_macros.hpp"

#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>

#include "simulator.hpp"

#include <cmath>

#define PI (4.*(atan(1.)))

dynamho_tests::dynamho_tests() : a(DataGenerator(21452)), yaml(generate_yaml())
{
}

dynamho_tests::~dynamho_tests()
{
}

void dynamho_tests::SetUp()
{
}

void dynamho_tests::TearDown()
{
}

typedef  Eigen::SelfAdjointView<Eigen::Matrix<double, 6, 6>, Eigen::Upper> InertiaMatrix;
typedef Eigen::Matrix<double, 6, 1> Force;

#define FORCE_MODULE(ns, x, code) namespace ns {namespace x{static const std::string signal_name = #ns "_" #x;MODULE(ForceModule, code)}}
#define APPEND_FORCE(x, ds) ds.add<x::ForceModule>(#x);\
	                        try\
	                        {\
	                        	ds.get<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name);\
                                }\
                                catch (const DataSourceException& exc)\
                                {\
                                    ds.set<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name,simulator_base::list_of_forces::_type());\
                                }\
                                    {\
                                        auto __v = ds.get<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name);\
                                        __v.insert(x::signal_name);\
                                        ds.set<simulator_base::list_of_forces::_type>(simulator_base::list_of_forces::_name,__v);\
                                    }

DEFINE(simulator_base, list_of_forces, std::set<std::string>)
DEFINE(dynamho, sim_start_stop, SimulationStartStopParameters)
DEFINE(dynamho, inertia_matrix, InertiaMatrix)
DEFINE(dynamho, inertia_parameters, Inertia)
DEFINE(dynamho, added_mass, InertiaMatrix)
DEFINE(dynamho, generalized_inertia_matrix, InertiaMatrix)
DEFINE(dynamho, hydrostatic, Force)
DEFINE(dynamho, inertial_coupling, Force)
DEFINE(dynamho, hydrodynamic, Force)
DEFINE(dynamho, hydrodynamic_parameters, ::hydrodynamic::Parameters)
DEFINE(dynamho, propulsion, Force)
DEFINE(dynamho, external, Force)
DEFINE(dynamho, sum_of_all_forces, Force)
DEFINE(dynamho, geometry, GeometryAndEnvironment)
DEFINE(dynamho, euler_angles, ::EulerAngles<double>)
DEFINE(dynamho, angles_de_barre, AngleDeBarre<double>)
DEFINE(dynamho, yaml, std::string)
DEFINE(dynamho, yaml_parser, std::tr1::shared_ptr<DynamhoYamlParser>)
DEFINE(dynamho, speed, Speed<double>)
DEFINE(dynamho, CzB2, double)
DEFINE(dynamho, Yb2B, double)

FORCE_MODULE(dynamho, hydrostatic, const auto I = PTR_GET(dynamho::inertia_parameters);\
                                   const auto G = PTR_GET(dynamho::geometry);\
                                   const ::EulerAngles<double> angles = PTR_GET(dynamho::euler_angles);\
                                   Force f;\
                                   f(0) = ::hydrostatic::model::Fx(I, G, angles);\
                                   f(1) = ::hydrostatic::model::Fy(I, G, angles);\
                                   f(2) = ::hydrostatic::model::Fz(I, G, angles);\
                                   f(3) = ::hydrostatic::model::Mx(I, G, angles);\
                                   f(4) = ::hydrostatic::model::My(I, G, angles);\
                                   f(5) = ::hydrostatic::model::Mz(I, G, angles);\
                                   PTR_SET(dynamho::hydrostatic, f))

FORCE_MODULE(dynamho, inertial_coupling,
                      const auto I = PTR_GET(dynamho::inertia_parameters);\
                      const auto G = PTR_GET(dynamho::geometry);\
                      const Speed<double> speed = PTR_GET(dynamho::speed);\
                      Force f;\
                      f(0) = ::inertia::model::Fx(I, G, speed);\
                      f(1) = ::inertia::model::Fy(I, G, speed);\
                      f(2) = ::inertia::model::Fz(I, G, speed);\
                      f(3) = ::inertia::model::Mx(I, G, speed);\
                      f(4) = ::inertia::model::My(I, G, speed);\
                      f(5) = ::inertia::model::Mz(I, G, speed);\
                      PTR_SET(dynamho::inertial_coupling, f))

FORCE_MODULE(dynamho, hydrodynamic,
                    const ::hydrodynamic::Parameters h_param = PTR_GET(dynamho::hydrodynamic_parameters);\
                    const GeometryAndEnvironment g_param = PTR_GET(dynamho::geometry);\
                    const AngleDeBarre<double> anglesDeBarres = PTR_GET(dynamho::angles_de_barre);\
                    const Speed<double> s = PTR_GET(dynamho::speed);\
                    const double CzB2 = PTR_GET(dynamho::CzB2);
                    const double Yb2B = PTR_GET(dynamho::Yb2B);
                    Force f;\
                    f(0) = ::hydrodynamic::model::Fx(h_param, g_param, anglesDeBarres, s);\
                    f(1) = ::hydrodynamic::model::Fy(h_param, g_param, anglesDeBarres, s);\
                    f(2) = ::hydrodynamic::model::Fz(h_param, g_param, anglesDeBarres, s);\
                    f(3) = ::hydrodynamic::model::Mx(h_param, g_param, anglesDeBarres, s, CzB2, Yb2B);\
                    f(4) = ::hydrodynamic::model::My(h_param, g_param, anglesDeBarres, s);\
                    f(5) = ::hydrodynamic::model::Mz(h_param, g_param, anglesDeBarres, s);\
                    PTR_SET(dynamho::hydrodynamic, f))

MODULE(dynamhoparser, const auto yaml = PTR_GET(dynamho::yaml);\
                      std::tr1::shared_ptr<DynamhoYamlParser> parser;
                      if (not(ds->read_only())) parser.reset(new DynamhoYamlParser(yaml));\
                      PTR_SET(dynamho::yaml_parser, parser);)

MODULE(inertia_parser, const auto parser = PTR_GET(dynamho::yaml_parser);\
                Inertia I;\
                if (not(ds->read_only())) I = parser->get_inertia();\
                PTR_SET(dynamho::inertia_parameters, I);)

MODULE(geometry_parser, const auto parser = PTR_GET(dynamho::yaml_parser);\
                 GeometryAndEnvironment G;\
                 if (not(ds->read_only())) G = parser->get_geometry();\
                 PTR_SET(dynamho::geometry, G);)

MODULE(hydrodynamic_parser, const auto parser = PTR_GET(dynamho::yaml_parser);\
       ::hydrodynamic::Parameters P;\
       if (not(ds->read_only())) P = parser->get_hydrodynamic();\
       PTR_SET(dynamho::hydrodynamic_parameters, P);)

MODULE(sum_of_all_forces, const auto forces = PTR_GET(simulator_base::list_of_forces);\
                          Force Ftot;
                          for (auto it = forces.begin() ; it != forces.end() ; ++it)\
                          {\
                              const auto F = ds->get<Force>(*it);\
                              Ftot += F;\
                          }\
                          PTR_SET(dynamho::sum_of_all_forces,Ftot);)

DataSource dynamho_tests::make_ds(const std::string& yaml_) const
{
    DataSource ds;
    SET(ds, dynamho::yaml, yaml_);
    ds.add<dynamhoparser>("parser");
    const auto parser = GET(ds, dynamho::yaml_parser);
    const auto start = parser->get_simulation_start_stop_parameters();

    SET(ds,dynamho::speed,start.initial_state.s);
    SET(ds,dynamho::CzB2,0);
    SET(ds,dynamho::Yb2B,0);
    SET(ds,dynamho::angles_de_barre,start.initial_angles);
    SET(ds,dynamho::euler_angles,start.initial_state.p.angle);

    ds.add<inertia_parser>("inertia_parser");
    ds.add<geometry_parser>("geometry_parser");
    ds.add<hydrodynamic_parser>("hydrodynamic_parser");
    ds.add<sum_of_all_forces>("Sum of forces");
    APPEND_FORCE(dynamho::hydrostatic, ds);
    APPEND_FORCE(dynamho::inertial_coupling, ds);
    APPEND_FORCE(dynamho::hydrodynamic, ds);
    return ds;
}

template<> EulerAngles<double> TypedScalarDataGenerator<EulerAngles<double> >::get() const
{
        EulerAngles<double> ret;
        ret.phi = random<double>().between(-100,100);
        ret.theta = random<double>().between(-100,100);
        ret.psi = random<double>().between(-100,100);
        return ret;
}

TEST_F(dynamho_tests, Fx_hydrostatic_should_be_zero_for_mu_equal_to_one)
{
    auto ds = make_ds(yaml);
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const EulerAngles<double> angles = a.random<EulerAngles<double> >();
        SET(ds,dynamho::euler_angles,angles);
        const Force F = GET(ds, dynamho::hydrostatic);
        ASSERT_DOUBLE_EQ(0, F(0));
    }

    auto I = GET(ds, dynamho::inertia_parameters);
    I.Mu = 0;
    FORCE(ds, dynamho::inertia_parameters, I);
}

TEST_F(dynamho_tests, Fx_hydrostatic_should_be_equal_to_g)
{
    auto ds = make_ds(yaml);
    const double expected = 60.82*99.44*0.9993*1026*9.81;
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        EulerAngles<double> angles = a.random<EulerAngles<double> >();
        angles.theta = PI/2.;
        SET(ds,dynamho::euler_angles,angles);
        auto I = GET(ds, dynamho::inertia_parameters);
        I.Mu = 0;
        FORCE(ds, dynamho::inertia_parameters, I);
        const Force F = GET(ds, dynamho::hydrostatic);
        ASSERT_DOUBLE_EQ(expected, F(0));
    }
}

TEST_F(dynamho_tests, can_retrieve_hydrostatic_forces)
{
    auto ds = make_ds(yaml);
    for (size_t i = 0 ; i < 1000 ; ++i)
    {
        const EulerAngles<double> angles(a.random<double>(),PI/4.,a.random<double>());
        SET(ds,dynamho::euler_angles,angles);
        const Force F = GET(ds, dynamho::hydrostatic);
        ASSERT_DOUBLE_EQ(0, F(0));
        ASSERT_DOUBLE_EQ(0, F(1));
        ASSERT_DOUBLE_EQ(0, F(2));
        ASSERT_DOUBLE_EQ(-0.3*9.81*cos(angles.theta)*sin(angles.phi)*60.82*99.44*0.9993*1026, F(3));
        ASSERT_DOUBLE_EQ(-0.3*9.81*sin(PI/4.)*60.82*99.44*0.9993*1026, F(4));
        ASSERT_DOUBLE_EQ(0, F(5));
    }
}

TEST_F(dynamho_tests, can_retrieve_inertial_coupling_forces)
{
    auto ds = make_ds(yaml);
    const Force F = GET(ds, dynamho::inertial_coupling);
}

TEST_F(dynamho_tests, can_retrieve_hydrodynamic_forces)
{
    auto ds = make_ds(yaml);
    const Force F = GET(ds, dynamho::hydrodynamic);
}

TEST_F(dynamho_tests, can_retrieve_sum_of_all_forces)
{
    auto ds = make_ds(yaml);
    const Force F = GET(ds, dynamho::sum_of_all_forces);
}

TEST_F(dynamho_tests, DISABLED_should_get_the_same_results_with_dynamho_and_DataSource)
{
//! [dynamho_tests example]
    const DynamhoYamlParser parser(yaml);
    DynamhoSimulation dynamho(parser.get_simulation_parameters());
    const auto out1 = dynamho.initialize(parser.get_simulation_start_stop_parameters());
    ASSERT_DOUBLE_EQ(0,out1.ship_states.p.coord.x);

    bool implemented = false;
//! [dynamho_tests example]
//! [dynamho_tests expected output]
    ASSERT_TRUE(implemented);
//! [dynamho_tests expected output]
}
