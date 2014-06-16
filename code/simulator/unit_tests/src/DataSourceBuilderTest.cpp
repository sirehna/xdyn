/*
 * DataSourceBuilderTest.cpp
 *
 *  Created on: 18 avr. 2014
 *      Author: cady
 */

#include "DataSourceBuilderTest.hpp"
#include "DataSourceBuilder.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"
#include "STL_data.hpp"
#include "EulerAngles.hpp"
#include "rotation_matrix_builders.hpp"
#include "Wrench.hpp"
#include "Mesh.hpp"
#include "PointMatrix.hpp"
#include "WaveModelInterface.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"
#include "StlReader.hpp"

#include <Eigen/Geometry>

typedef Eigen::Matrix<double,6,6> Matrix6x6;

DataSourceBuilderTest::DataSourceBuilderTest() : a(DataGenerator(12)), ds(DataSource()), mesh_data(read_stl(test_data::three_facets()))
{
    const std::map<std::string, VectorOfVectorOfPoints> m = { {"body 1", mesh_data} };
    const SimulatorYamlParser parser(test_data::full_example());
    DataSourceBuilder builder(parser.parse(), m);
    ds = builder.build_ds();
}

DataSourceBuilderTest::~DataSourceBuilderTest()
{
}

void DataSourceBuilderTest::SetUp()
{
}

void DataSourceBuilderTest::TearDown()
{
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_initial_states)
{
    ASSERT_DOUBLE_EQ(4,ds.get<double>("x(body 1)"));
    ASSERT_DOUBLE_EQ(8,ds.get<double>("y(body 1)"));
    ASSERT_DOUBLE_EQ(12,ds.get<double>("z(body 1)"));
    ASSERT_DOUBLE_EQ(-8,ds.get<double>("u(body 1)"));
    ASSERT_DOUBLE_EQ(-9,ds.get<double>("v(body 1)"));
    ASSERT_DOUBLE_EQ(14,ds.get<double>("w(body 1)"));
    ASSERT_DOUBLE_EQ(56,ds.get<double>("p(body 1)"));
    ASSERT_DOUBLE_EQ(7,ds.get<double>("q(body 1)"));
    ASSERT_DOUBLE_EQ(6,ds.get<double>("r(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_initial_quaternions)
{
    const EulerAngles expected(13,14,15);
    const Eigen::Quaternion<double> q(kinematics::rotation_matrix<kinematics::INTRINSIC, kinematics::CHANGING_ANGLE_ORDER, kinematics::CARDAN, 3, 2, 1>(expected));

    ASSERT_DOUBLE_EQ(q.w(),ds.get<double>("qr(body 1)"));
    ASSERT_DOUBLE_EQ(q.x(),ds.get<double>("qi(body 1)"));
    ASSERT_DOUBLE_EQ(q.y(),ds.get<double>("qj(body 1)"));
    ASSERT_DOUBLE_EQ(q.z(),ds.get<double>("qk(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_the_right_number_of_states)
{
    ASSERT_EQ(13, ds.get_state_names().size());
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_the_gravity_constant)
{
    ASSERT_DOUBLE_EQ(9.81, ds.get<double>("g"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_the_gravity_force_of_each_body)
{
    const Wrench Fg = ds.get<Wrench>("gravity(body 1)");
    ASSERT_DOUBLE_EQ(0,Fg.X);
    ASSERT_DOUBLE_EQ(0,Fg.Y);
    ASSERT_DOUBLE_EQ(9.81E6,Fg.Z);
    ASSERT_DOUBLE_EQ(0,Fg.K);
    ASSERT_DOUBLE_EQ(0,Fg.M);
    ASSERT_DOUBLE_EQ(0,Fg.N);
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_the_mass_of_each_body)
{
    ASSERT_DOUBLE_EQ(1E6, ds.get<double>("m(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_mesh_of_each_body)
{
    const TR1(shared_ptr)<Mesh> m = ds.get<TR1(shared_ptr)<Mesh> >("body 1");
    ASSERT_EQ(3, m->facets.size());
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_a_PointMatrix_for_each_body)
{
    const TR1(shared_ptr)<PointMatrix> P = ds.get<TR1(shared_ptr)<PointMatrix> >("body 1");
    ASSERT_EQ(3, P->m.rows());
    ASSERT_EQ(7, P->m.cols());
    ASSERT_EQ("mesh(body 1)", P->get_frame());
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_centre_of_gravity_of_each_body)
{
    const Point G = ds.get<Point>("G(body 1)");
    ASSERT_EQ("body 1", G.get_frame());
    ASSERT_DOUBLE_EQ(4, G.x);
    ASSERT_DOUBLE_EQ(7, G.y);
    ASSERT_DOUBLE_EQ(-10, G.z);
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_a_Kinematics_object)
{
    TR1(shared_ptr)<Kinematics> k = ds.get<TR1(shared_ptr)<Kinematics> >("kinematics");
    kinematics::Transform T = k->get("NED", "body 1");
    Point P = T*Point("NED",0,0,0);
    ASSERT_EQ("body 1", P.get_frame());
    ASSERT_DOUBLE_EQ(4, P.x);
    ASSERT_DOUBLE_EQ(8, P.y);
    ASSERT_DOUBLE_EQ(12, P.z);
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double x = a.random<double>();
        ds.set<double>("x(body 1)",x);
        k = ds.get<TR1(shared_ptr)<Kinematics> >("kinematics");
        T = k->get("NED", "body 1");
        P = T*Point("NED",0,0,0);
        ASSERT_EQ("body 1", P.get_frame());
        ASSERT_DOUBLE_EQ(x, P.x);
    }
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_position_of_body_relative_to_mesh)
{
    ASSERT_NO_THROW(ds.get<double>("x(body 1/mesh)"));
    ASSERT_NO_THROW(ds.get<double>("y(body 1/mesh)"));
    ASSERT_NO_THROW(ds.get<double>("z(body 1/mesh)"));
    ASSERT_NO_THROW(ds.get<RotationMatrix>("body 1/mesh"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_wave_model)
{
    const TR1(shared_ptr)<WaveModelInterface> wave_model = ds.get<TR1(shared_ptr)<WaveModelInterface> >("wave model");
    const TR1(shared_ptr)<Kinematics> k = ds.get<TR1(shared_ptr)<Kinematics> >("kinematics");
    for (size_t i = 0 ; i < 100 ; ++i)
    {
        const double z0 = a.random<double>();
        const Point P("NED",a.random<double>(),a.random<double>(),z0);
        const Point Q("body 1",a.random<double>(),a.random<double>(),z0);
        ASSERT_DOUBLE_EQ(z0-12, wave_model->get_relative_wave_height(P,k));
        ASSERT_NO_THROW(wave_model->get_relative_wave_height(Q,k));
    }
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_hydrostatic_forces_for_each_body)
{
    ASSERT_NO_THROW(ds.get<Wrench>("non-linear hydrostatic(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_total_inertia_matrix_of_each_body)
{
    ASSERT_NO_THROW(ds.get<Matrix6x6>("total inertia(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_added_mass_matrix_of_each_body)
{
    ASSERT_NO_THROW(ds.get<Matrix6x6>("solid body inertia(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_inertia_even_when_body_is_not_named_body_1)
{
    const SimulatorYamlParser parser(test_data::falling_ball_example());
    DataSourceBuilder b(parser.parse());
    DataSource d = b.build_ds();
    ASSERT_NO_THROW(d.get<Matrix6x6>("total inertia(ball)"));
    ASSERT_NO_THROW(d.get<Matrix6x6>("solid body inertia(ball)"));
    ASSERT_NO_THROW(d.get<Matrix6x6>("inverse of the total inertia(ball)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_the_inverse_of_the_total_inertia_matrix)
{
    const Matrix6x6 M = ds.get<Matrix6x6>("total inertia(body 1)");
    const Matrix6x6 M_inv = ds.get<Matrix6x6>("inverse of the total inertia(body 1)");
    const Matrix6x6 I1(M*M_inv);
    const Matrix6x6 I2(M_inv*M);

    for (size_t i = 0 ; i < 6 ; ++i)
    {
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ASSERT_NEAR(i==j ? 1 : 0, (double)I1(i,j),1E-10);
            ASSERT_NEAR(i==j ? 1 : 0, (double)I2(i,j),1E-10);
        }
    }
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_sum_of_forces_for_each_body)
{
    ASSERT_NO_THROW(ds.get<Wrench>("sum of forces(body 1)"));
}

TEST_F(DataSourceBuilderTest, DataSource_should_contain_coriolis_and_centripetal_forces_for_each_body)
{
    const Wrench F = ds.get<Wrench>("coriolis and centripetal forces(body 1)");
    const Point C = ds.get<Point>("G(body 1)");
    ASSERT_EQ(C.get_frame(), F.get_frame());
    ASSERT_EQ(C.x, F.get_point().x);
    ASSERT_EQ(C.y, F.get_point().y);
    ASSERT_EQ(C.z, F.get_point().z);
}
