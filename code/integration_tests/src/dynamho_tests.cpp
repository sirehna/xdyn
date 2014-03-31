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
#include "extra_test_assertions.hpp"
#include "DsSolve.hpp"
#include "DsCsvObserver.hpp"
#include "DsSystemMacros.hpp"

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

typedef Eigen::Matrix<double, 6, 6> InertiaMatrix;
typedef Eigen::Matrix<double, 3, 3> RotationMatrix;
typedef Eigen::Matrix<double, 6, 1> Force;
typedef Eigen::Matrix<double, 6, 1> Uvwpqr;


DEFINE(dynamho, inertia_matrix, InertiaMatrix)
DEFINE(dynamho, inertia_parameters, Inertia)
DEFINE(dynamho, added_mass, InertiaMatrix)
DEFINE(dynamho, hydrostatic, Force)
DEFINE(dynamho, inertial_coupling, Force)
DEFINE(dynamho, hydrodynamic, Force)
DEFINE(dynamho, hydrodynamic_parameters, ::hydrodynamic::Parameters)
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
DEFINE(dynamho, state_derivatives, State<double>)
DEFINE(dynamho, pqr2phithetapsi, RotationMatrix)
DEFINE(dynamho, uvw2xyz_dot, RotationMatrix)
DEFINE(dynamho, uvwpqr_dot, Uvwpqr)
DEFINE(dynamho, xyz_dot, Eigen::Vector3d)
DEFINE(dynamho, phithetapsi_dot, Eigen::Vector3d)
DEFINE(dynamho, dx_dt, double)
DEFINE(dynamho, dy_dt, double)
DEFINE(dynamho, dz_dt, double)
DEFINE(dynamho, dphi_dt, double)
DEFINE(dynamho, dtheta_dt, double)
DEFINE(dynamho, dpsi_dt, double)
DEFINE(dynamho, du_dt, double)
DEFINE(dynamho, dv_dt, double)
DEFINE(dynamho, dw_dt, double)
DEFINE(dynamho, dp_dt, double)
DEFINE(dynamho, dq_dt, double)
DEFINE(dynamho, dr_dt, double)
DEFINE(dynamho, x, double)
DEFINE(dynamho, y, double)
DEFINE(dynamho, z, double)
DEFINE(dynamho, phi, double)
DEFINE(dynamho, theta, double)
DEFINE(dynamho, psi, double)
DEFINE(dynamho, u, double)
DEFINE(dynamho, v, double)
DEFINE(dynamho, w, double)
DEFINE(dynamho, p, double)
DEFINE(dynamho, q, double)
DEFINE(dynamho, r, double)

FORCE_MODULE(dynamho, hydrostatic, const auto I = PTR_GET(dynamho::inertia_parameters);\
                                   const auto G = PTR_GET(dynamho::geometry);\
                                   ::EulerAngles<double> angles;\
                                   angles.phi = PTR_GET(dynamho::phi);\
                                   angles.theta = PTR_GET(dynamho::theta);\
                                   angles.psi = PTR_GET(dynamho::psi);\
                                   Force fhydrostat;\
                                   fhydrostat(0) = ::hydrostatic::model::Fx(I, G, angles);\
                                   fhydrostat(1) = ::hydrostatic::model::Fy(I, G, angles);\
                                   fhydrostat(2) = ::hydrostatic::model::Fz(I, G, angles);\
                                   fhydrostat(3) = ::hydrostatic::model::Mx(I, G, angles);\
                                   fhydrostat(4) = ::hydrostatic::model::My(I, G, angles);\
                                   fhydrostat(5) = ::hydrostatic::model::Mz(I, G, angles);\
                                   PTR_SET(dynamho::hydrostatic, fhydrostat))

FORCE_MODULE(dynamho, inertial_coupling,
                      const auto I = PTR_GET(dynamho::inertia_parameters);\
                      const auto G = PTR_GET(dynamho::geometry);\
                      Speed<double> s;\
                      s.trans.u = PTR_GET(dynamho::u);\
                      s.trans.v = PTR_GET(dynamho::v);\
                      s.trans.w = PTR_GET(dynamho::w);\
                      s.rot.p = PTR_GET(dynamho::p);\
                      s.rot.q = PTR_GET(dynamho::q);\
                      s.rot.r = PTR_GET(dynamho::r);\
                      Force finertia;\
                      finertia(0) = ::inertia::model::Fx(I, G, s);\
                      finertia(1) = ::inertia::model::Fy(I, G, s);\
                      finertia(2) = ::inertia::model::Fz(I, G, s);\
                      finertia(3) = ::inertia::model::Mx(I, G, s);\
                      finertia(4) = ::inertia::model::My(I, G, s);\
                      finertia(5) = ::inertia::model::Mz(I, G, s);\
                      PTR_SET(dynamho::inertial_coupling, finertia))

FORCE_MODULE(dynamho, hydrodynamic,
                    const ::hydrodynamic::Parameters h_param = PTR_GET(dynamho::hydrodynamic_parameters);\
                    const GeometryAndEnvironment g_param = PTR_GET(dynamho::geometry);\
                    const AngleDeBarre<double> anglesDeBarres = PTR_GET(dynamho::angles_de_barre);\
                    Speed<double> s;\
                    s.trans.u = PTR_GET(dynamho::u);\
                    s.trans.v = PTR_GET(dynamho::v);\
                    s.trans.w = PTR_GET(dynamho::w);\
                    s.rot.p = PTR_GET(dynamho::p);\
                    s.rot.q = PTR_GET(dynamho::q);\
                    s.rot.r = PTR_GET(dynamho::r);\
                    const double CzB2 = PTR_GET(dynamho::CzB2);
                    const double Yb2B = PTR_GET(dynamho::Yb2B);
                    Force fhydrodynamic;\
                    fhydrodynamic(0) = ::hydrodynamic::model::Fx(h_param, g_param, anglesDeBarres, s);\
                    fhydrodynamic(1) = ::hydrodynamic::model::Fy(h_param, g_param, anglesDeBarres, s);\
                    fhydrodynamic(2) = ::hydrodynamic::model::Fz(h_param, g_param, anglesDeBarres, s);\
                    fhydrodynamic(3) = ::hydrodynamic::model::Mx(h_param, g_param, anglesDeBarres, s, CzB2, Yb2B);\
                    fhydrodynamic(4) = ::hydrodynamic::model::My(h_param, g_param, anglesDeBarres, s);\
                    fhydrodynamic(5) = ::hydrodynamic::model::Mz(h_param, g_param, anglesDeBarres, s);\
                    PTR_SET(dynamho::hydrodynamic, fhydrodynamic))

MODULE(dynamhoparser, const auto yaml = PTR_GET(dynamho::yaml);\
                      std::tr1::shared_ptr<DynamhoYamlParser> parser;
                      if (not(ds->read_only())) parser.reset(new DynamhoYamlParser(yaml));\
                      PTR_SET(dynamho::yaml_parser, parser);)

MODULE(rotationmatrix, const double phi = PTR_GET(dynamho::phi);\
                       const double theta = PTR_GET(dynamho::theta);\
                       RotationMatrix R;\
                       R(0,0) = 1;\
                       R(0,1) = tan(theta)*sin(phi);\
                       R(0,2) = tan(theta)*cos(phi);\
                       R(1,0) = 0;\
                       R(1,1) = cos(phi);\
                       R(1,2) = -sin(phi);\
                       R(2,0) = 0;\
                       R(2,1) = sin(phi)/cos(theta);\
                       R(2,2) = cos(phi)/cos(theta);\
                       PTR_SET(dynamho::pqr2phithetapsi, R);)


MODULE(uvw2xyz_dot,   const double phi = PTR_GET(dynamho::phi);\
                      const double theta = PTR_GET(dynamho::theta);\
                      const double psi = PTR_GET(dynamho::psi);\
                      RotationMatrix R;\
                      R(0,0) = cos(psi)*cos(theta);\
                      R(0,1) = cos(psi)*sin(theta)*sin(phi)-sin(psi)*cos(phi);\
                      R(0,2) = sin(psi)*sin(phi)+cos(psi)*sin(theta)*cos(phi);\
                      R(1,0) = sin(psi)*cos(theta);\
                      R(1,1) = cos(psi)*cos(phi)+sin(psi)*sin(theta)*sin(phi);\
                      R(1,2) = sin(psi)*sin(theta)*cos(phi)-cos(psi)*sin(phi);\
                      R(2,0) = -sin(theta);\
                      R(2,1) = cos(theta)*sin(phi);\
                      R(2,2) = cos(theta)*cos(phi);\
                      PTR_SET(dynamho::uvw2xyz_dot, R);)

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
                          Force Ftot;\
                          Ftot(0,0) = 0;\
                          Ftot(1,0) = 0;\
                          Ftot(2,0) = 0;\
                          Ftot(3,0) = 0;\
                          Ftot(4,0) = 0;\
                          Ftot(5,0) = 0;\
                          for (auto it = forces.begin() ; it != forces.end() ; ++it)\
                          {\
                              const auto F = ds->get<Force>(*it);\
                              Ftot += F;\
                          }\
                          PTR_SET(dynamho::sum_of_all_forces,Ftot);)

MODULE(kinematics, const auto R1 = PTR_GET(dynamho::uvw2xyz_dot);\
                   const auto R2 = PTR_GET(dynamho::pqr2phithetapsi);\
                   Eigen::Vector3d pqr;\
                   pqr(0) = PTR_GET(dynamho::p);\
                   pqr(1) = PTR_GET(dynamho::q);\
                   pqr(2) = PTR_GET(dynamho::r);\
                   Eigen::Vector3d uvw;\
                   uvw(0) = PTR_GET(dynamho::u);\
                   uvw(1) = PTR_GET(dynamho::v);\
                   uvw(2) = PTR_GET(dynamho::w);\
                   Eigen::Vector3d phithetapsi_dot;\
                   Eigen::Vector3d xyz_dot;\
                   if (not(ds->read_only())) xyz_dot = R1*uvw;\
                   if (not(ds->read_only())) phithetapsi_dot = R2*pqr;\
                   PTR_SET(dynamho::xyz_dot,xyz_dot);\
                   PTR_SET(dynamho::phithetapsi_dot,phithetapsi_dot);\
                   )

MODULE(dynamics, const auto M = PTR_GET(dynamho::inertia_matrix);\
                 const auto F = PTR_GET(dynamho::sum_of_all_forces);\
                 Uvwpqr uvwpqr_dot;\
                 if (not(ds->read_only())) uvwpqr_dot = M.llt().solve(F);\
                 PTR_SET(dynamho::uvwpqr_dot, uvwpqr_dot);\
       )

MODULE(scalarize, const auto xyz_dot = PTR_GET(dynamho::xyz_dot);\
                  const auto phithetapsi_dot = PTR_GET(dynamho::phithetapsi_dot);\
                  const auto uvwpqr_dot = PTR_GET(dynamho::uvwpqr_dot);\
                  PTR_SET(dynamho::dx_dt, xyz_dot(0));\
                  PTR_SET(dynamho::dy_dt, xyz_dot(1));\
                  PTR_SET(dynamho::dz_dt, xyz_dot(2));\
                  PTR_SET(dynamho::dphi_dt, phithetapsi_dot(0));\
                  PTR_SET(dynamho::dtheta_dt, phithetapsi_dot(1));\
                  PTR_SET(dynamho::dpsi_dt, phithetapsi_dot(2));\
                  PTR_SET(dynamho::du_dt, uvwpqr_dot(0));\
                  PTR_SET(dynamho::dv_dt, uvwpqr_dot(1));\
                  PTR_SET(dynamho::dw_dt, uvwpqr_dot(2));\
                  PTR_SET(dynamho::dp_dt, uvwpqr_dot(3));\
                  PTR_SET(dynamho::dq_dt, uvwpqr_dot(4));\
                  PTR_SET(dynamho::dr_dt, uvwpqr_dot(5));\
)

MODULE(vectorize, State<double> state_derivative;\
                  state_derivative.p.coord.x = PTR_GET(dynamho::dx_dt);\
                  state_derivative.p.coord.y = PTR_GET(dynamho::dy_dt);\
                  state_derivative.p.coord.z = PTR_GET(dynamho::dz_dt);\
                  state_derivative.p.angle.phi = PTR_GET(dynamho::dphi_dt);\
                  state_derivative.p.angle.theta = PTR_GET(dynamho::dtheta_dt);\
                  state_derivative.p.angle.psi = PTR_GET(dynamho::dpsi_dt);\
                  state_derivative.s.trans.u = PTR_GET(dynamho::du_dt);\
                  state_derivative.s.trans.v = PTR_GET(dynamho::dv_dt);\
                  state_derivative.s.trans.w = PTR_GET(dynamho::dw_dt);\
                  state_derivative.s.rot.p = PTR_GET(dynamho::dp_dt);\
                  state_derivative.s.rot.q = PTR_GET(dynamho::dq_dt);\
                  state_derivative.s.rot.r = PTR_GET(dynamho::dr_dt);\
                  PTR_SET(dynamho::state_derivatives,state_derivative);\
                  )

MODULE(inertia_computer, InertiaMatrix mat = InertiaMatrix::Zero();\
const GeometryAndEnvironment G = PTR_GET(dynamho::geometry);\
const auto I = PTR_GET(dynamho::inertia_parameters);\
const double Mu_1 = I.Mu_1;\
const double lambda_1 = I.Lambda_1;\
const double lambda_2 = I.Lambda_2;\
const double lambda_3 = I.Lambda_3;\
const double lambda_13 = I.Lambda_13;\
const double Nu_24 = I.Nu_24;\
const double Nu_15 = I.Nu_15;\
const double Nu_35 = I.Nu_35;\
const double Nu_26 = I.Nu_26;\
const double Mu = I.Mu;\
const double Xg = G.Xg;\
const double Mu_13 = I.Mu_13;\
const double Mu_2 = I.Mu_2;\
const double Mu_3 = I.Mu_3;\
const double Yg = G.Yg;\
const double Zg = G.Zg;\
const double X_1 = I.X_1;\
const double X_2 = I.X_2;\
const double X_13 = I.X_13;\
const double X_3 = I.X_3;\
const double L = G.L;\
    mat(0,0)=Mu+Mu_1;\
    mat(0,2)=Mu_13;\
    mat(1,1)=Mu+Mu_2;\
    mat(2,0)=mat(0,2);\
    mat(2,2)=Mu+Mu_3;\
    mat(3,1)=-Mu*Zg-L*Nu_24;\
    mat(3,2)= Mu*Yg;\
    mat(4,0)= Mu*Zg-L*Nu_15;\
    mat(4,2)=-Mu*Xg-L*Nu_35;\
    mat(5,0)=-Mu*Yg;\
    mat(5,1)= Mu*Xg-L*Nu_26;\
    mat(0,4)=mat(4,0);\
    mat(0,5)=mat(5,0);\
    mat(1,3)=mat(3,1);\
    mat(1,5)=mat(5,1);\
    mat(2,3)=mat(3,2);\
    mat(2,4)=mat(4,2);\
    mat(3,3)= L*L*(X_1+lambda_1);\
    mat(3,5)=-L*L*(X_13+lambda_13);\
    mat(4,4)= L*L*(X_2+lambda_2);\
    mat(5,3)= mat(3,5);\
    mat(5,5)= L*L*(X_3+lambda_3);\
 PTR_SET(dynamho::inertia_matrix, mat);\
)

DataSource dynamho_tests::make_ds(const std::string& yaml_) const
{
    DataSource ds;
    SET(ds, dynamho::yaml, yaml_);
    ds.add<dynamhoparser>("parser");
    const auto parser = GET(ds, dynamho::yaml_parser);
    const auto start = parser->get_simulation_start_stop_parameters();
    initialize_DS_with_yaml_start_parameters(ds, start);

    DEFINE_DERIVATIVE(dynamho::x, dynamho::dx_dt, ds);
    DEFINE_DERIVATIVE(dynamho::y, dynamho::dy_dt, ds);
    DEFINE_DERIVATIVE(dynamho::z, dynamho::dz_dt, ds);
    DEFINE_DERIVATIVE(dynamho::phi, dynamho::dphi_dt, ds);
    DEFINE_DERIVATIVE(dynamho::theta, dynamho::dtheta_dt, ds);
    DEFINE_DERIVATIVE(dynamho::psi, dynamho::dpsi_dt, ds);
    DEFINE_DERIVATIVE(dynamho::u, dynamho::du_dt, ds);
    DEFINE_DERIVATIVE(dynamho::v, dynamho::dv_dt, ds);
    DEFINE_DERIVATIVE(dynamho::w, dynamho::dw_dt, ds);
    DEFINE_DERIVATIVE(dynamho::p, dynamho::dp_dt, ds);
    DEFINE_DERIVATIVE(dynamho::q, dynamho::dq_dt, ds);
    DEFINE_DERIVATIVE(dynamho::r, dynamho::dr_dt, ds);

    ds.add<inertia_parser>("inertia_parser");
    ds.add<geometry_parser>("geometry_parser");
    ds.add<hydrodynamic_parser>("hydrodynamic_parser");
    ds.add<sum_of_all_forces>("Sum of forces");
    ds.add<rotationmatrix>("From p,q,r to phi_dot,theta_dot,psi_dot");
    ds.add<uvw2xyz_dot>("From u,v,w to x_dot,y_dot,z_dot");
    ds.add<kinematics>("Kinematics");
    ds.add<dynamics>("Dynamic model");
    ds.add<vectorize>("Vectorize");
    ds.add<scalarize>("Scalarize");
    ds.add<inertia_computer>("Inertia Matrix");
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
    for (size_t i = 0 ; i < 100 ; ++i)
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
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        EulerAngles<double> angles = a.random<EulerAngles<double> >();
        angles.theta = PI/2.;
        SET(ds,dynamho::phi,angles.phi);
        SET(ds,dynamho::theta,angles.theta);
        SET(ds,dynamho::psi,angles.psi);
        auto I = GET(ds, dynamho::inertia_parameters);
        I.Mu = 0;
        FORCE(ds, dynamho::inertia_parameters, I);
        const Force F = GET(ds, dynamho::hydrostatic);
        ASSERT_DOUBLE_EQ(expected, F(0));
    }
}

TEST_F(dynamho_tests, can_retrieve_hydrostatic_forces)
{
    const double EPS = 5E-3;
    auto ds = make_ds(yaml);
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const EulerAngles<double> angles(a.random<double>(),PI/4.,a.random<double>());
        SET(ds,dynamho::phi,angles.phi);
        SET(ds,dynamho::theta,angles.theta);
        SET(ds,dynamho::psi,angles.psi);
        const Force F = GET(ds, dynamho::hydrostatic);
        ASSERT_DOUBLE_EQ(0, F(0));
        ASSERT_DOUBLE_EQ(0, F(1));
        ASSERT_DOUBLE_EQ(0, F(2));
        ASSERT_NEAR(-0.3*9.81*cos(angles.theta)*sin(angles.phi)*60.82*99.44*0.9993*1026, F(3),EPS);
        ASSERT_NEAR(-0.3*9.81*sin(PI/4.)*60.82*99.44*0.9993*1026, F(4),EPS);
        ASSERT_DOUBLE_EQ(0, F(5));
    }
}
/*
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

TEST_F(dynamho_tests, can_retrieve_X_dot)
{
    auto ds = make_ds(yaml);
    const State<double> x_dot = GET(ds, dynamho::state_derivatives);
}

TEST_F(dynamho_tests, can_retrieve_the_matrix_pqr2phithetapsi)
{
    auto ds = make_ds(yaml);
    const auto R = GET(ds, dynamho::pqr2phithetapsi);
}

TEST_F(dynamho_tests, can_retrieve_the_matrix_uvw2xyz_dot)
{
    auto ds = make_ds(yaml);
    const auto R = GET(ds, dynamho::uvw2xyz_dot);
}
*/
TEST_F(dynamho_tests, should_get_the_same_initialization_with_dynamho_and_DataSource)
{
//! [dynamho_tests example]
    const DynamhoYamlParser parser(yaml);
    DynamhoSimulation dynamho(parser.get_simulation_parameters());
    const auto out1 = dynamho.initialize(parser.get_simulation_start_stop_parameters());
    ASSERT_DOUBLE_EQ(0,out1.ship_states.p.coord.x);
    auto ds = make_ds(yaml);
//! [dynamho_tests example]
//! [dynamho_tests expected output]
    const State<double> x_dot = GET(ds, dynamho::state_derivatives);
    ASSERT_DOUBLE_EQ(parser.get_simulation_start_stop_parameters().initial_state.s.trans.u,x_dot.p.coord.x);
    ASSERT_DOUBLE_EQ(parser.get_simulation_start_stop_parameters().initial_state.s.trans.v,x_dot.p.coord.y);
    ASSERT_DOUBLE_EQ(parser.get_simulation_start_stop_parameters().initial_state.s.trans.w,x_dot.p.coord.z);
//! [dynamho_tests expected output]
}

void dynamho_tests::initialize_DS_with_yaml_start_parameters(DataSource& ds, const SimulationStartStopParameters& ss) const
{
    SET(ds,dynamho::angles_de_barre, ss.initial_angles);
    SET(ds,dynamho::CzB2,0);
    SET(ds,dynamho::Yb2B,0);
    SET(ds, dynamho::x, ss.initial_state.p.coord.x);
    SET(ds, dynamho::y, ss.initial_state.p.coord.y);
    SET(ds, dynamho::z, ss.initial_state.p.coord.z);
    SET(ds, dynamho::phi, ss.initial_state.p.angle.phi);
    SET(ds, dynamho::theta, ss.initial_state.p.angle.theta);
    SET(ds, dynamho::psi, ss.initial_state.p.angle.psi);
    SET(ds, dynamho::u, ss.initial_state.s.trans.u);
    SET(ds, dynamho::v, ss.initial_state.s.trans.v);
    SET(ds, dynamho::w, ss.initial_state.s.trans.w);
    SET(ds, dynamho::p, ss.initial_state.s.rot.p);
    SET(ds, dynamho::q, ss.initial_state.s.rot.q);
    SET(ds, dynamho::r, ss.initial_state.s.rot.r);
}

TEST_F(dynamho_tests, same_results_with_dynamho_and_DataSource_rk4)
{
    const DynamhoYamlParser parser(yaml);
    auto par = parser.get_simulation_parameters();
    par.simulation.initial_state.s.trans.u = 1;
    par.simulation.initial_state.p.coord.z = 100;
    par.simulation.Tmax = 1;
    par.simulation.time_step = 1;
    par.simulation.solver = "RK4";
    par.tables.propulsion_data.D = 0;
    DynamhoSimulation dynamho(par);

    const auto out = dynamho.step(AngleDeBarre<double>());
    auto ds = make_ds(yaml);
    initialize_DS_with_yaml_start_parameters(ds, par.simulation);
    SET(ds, simulator_base::initial_time_step, 1);
    SET(ds, simulator_base::stepper, solver::RK4);
    std::stringstream ss;
    DsCsvObserver observer(std::cout);
    integrate(ds, 0, 1, observer);

    const double eps = 1e-6;

    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.p.coord.x, GET(ds, dynamho::x),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.p.coord.y, GET(ds, dynamho::y),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.p.coord.z, GET(ds, dynamho::z),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.p.angle.phi, GET(ds, dynamho::phi),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.p.angle.theta, GET(ds, dynamho::theta),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.p.angle.psi, GET(ds, dynamho::psi),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.s.trans.u, GET(ds, dynamho::u),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.s.trans.v, GET(ds, dynamho::v),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.s.trans.w, GET(ds, dynamho::w),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.s.rot.p, GET(ds, dynamho::p),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.s.rot.q, GET(ds, dynamho::q),eps);
    ASSERT_SMALL_RELATIVE_ERROR(out.ship_states.s.rot.r, GET(ds, dynamho::r),eps);

}
