/*
 * SimulatorBuilderTest.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: cady
 */

#include "SimulatorBuilderTest.hpp"
#include "SimulatorBuilder.hpp"
#include "SimulatorBuilderException.hpp"
#include "TriMeshTestData.hpp"
#include "generate_body_for_tests.hpp"
#include "Kinematics.hpp"
#include "Transform.hpp"
#include "DefaultWaveModel.hpp"
#include "GravityForceModel.hpp"
#include "HydrostaticForceModel.hpp"
#include "SimulatorYamlParser.hpp"
#include "yaml_data.hpp"

const YamlSimulatorInput SimulatorBuilderTest::input = SimulatorYamlParser(test_data::full_example()).parse();


SimulatorBuilderTest::SimulatorBuilderTest() : a(DataGenerator(1212)),
                                               builder(SimulatorBuilder(input))
{
}

SimulatorBuilderTest::~SimulatorBuilderTest()
{
}

void SimulatorBuilderTest::SetUp()
{
}

void SimulatorBuilderTest::TearDown()
{
}

TEST_F(SimulatorBuilderTest, throws_if_cannot_find_mesh)
{
    ASSERT_THROW(builder.get_bodies(MeshMap()),SimulatorBuilderException);
}

TEST_F(SimulatorBuilderTest, can_get_bodies)
{
    MeshMap m;
    m[input.bodies.front().name] = two_triangles();
    const auto bodies = builder.get_bodies(m);
    ASSERT_EQ(1, bodies.size());
    ASSERT_EQ(input.bodies.front().name, bodies.front().name);
    const auto Id = (*bodies.front().inverse_of_the_total_inertia)*(*bodies.front().total_inertia);
    for (size_t i = 0 ; i < 6 ; ++i)
    {
        for (size_t j = 0 ; j < 6 ; ++j)
        {
            ASSERT_NEAR(Id(i,j), i==j, 1E-14) << "i = " << i << ", j = " << j;
        }
    }
}

TEST_F(SimulatorBuilderTest, can_get_rho_and_g)
{
    builder.can_parse<DefaultWaveModel>();
    const auto env = builder.get_environment_and_frames(std::vector<Body>(1,get_body(a.random<std::string>())));
    ASSERT_DOUBLE_EQ(9.81,env.g);
    ASSERT_DOUBLE_EQ(1000,env.rho);
}

std::string SimulatorBuilderTest::customize(const std::string& body_name, const std::string& something) const
{
    return something + "(" + body_name + ")";
}

TEST_F(SimulatorBuilderTest, kinematics_contains_body_to_mesh_transform)
{
    builder.can_parse<DefaultWaveModel>();
    const std::vector<Body> bodies(1,get_body(a.random<std::string>()));
    const auto env = builder.get_environment_and_frames(bodies);
    ASSERT_TRUE(env.k.get() != NULL);
    for (auto that_body = bodies.begin() ; that_body != bodies.end() ; ++that_body)
    {
        ASSERT_NO_THROW(env.k->get(that_body->name, customize(that_body->name, "mesh")));
    }
}

TEST_F(SimulatorBuilderTest, kinematics_contains_ned_to_body_transform)
{
    builder.can_parse<DefaultWaveModel>();
    const std::vector<Body> bodies(1,get_body(a.random<std::string>()));
    const auto env = builder.get_environment_and_frames(bodies);
    ASSERT_TRUE(env.k.get() != NULL);
    for (auto that_body = bodies.begin() ; that_body != bodies.end() ; ++that_body)
    {
        ASSERT_NO_THROW(env.k->get("NED", that_body->name));
    }
}

TEST_F(SimulatorBuilderTest, should_throw_if_no_wave_parser_defined)
{
    ASSERT_THROW(builder.get_environment_and_frames(std::vector<Body>()), SimulatorBuilderException);
}

TEST_F(SimulatorBuilderTest, should_throw_if_attempting_to_define_wave_model_twice)
{
    YamlModel model;
    model.model = "no waves";
    model.yaml = "constant wave height in NED frame:\n   unit: m\n   value: 12";
    auto input2 = input;
    input2.environment.push_back(model);
    SimulatorBuilder builder2(input2);
    builder2.can_parse<DefaultWaveModel>();
    ASSERT_THROW(builder2.get_environment_and_frames(std::vector<Body>()), SimulatorBuilderException);
}

TEST_F(SimulatorBuilderTest, can_get_waves)
{
    builder.can_parse<DefaultWaveModel>();
    MeshMap m;
    const std::string name = input.bodies.front().name;
    m[name] = two_triangles();
    const auto bodies = builder.get_bodies(m);
    const auto env = builder.get_environment_and_frames(bodies);
    ASSERT_TRUE(env.w.get() != NULL);
    const Point P("NED",1,2,3);
    ASSERT_DOUBLE_EQ(3-12, env.w->get_relative_wave_height(P,env.k));
}

TEST_F(SimulatorBuilderTest, get_forces_should_throw_if_there_is_anything_it_cannot_parse)
{
    builder.can_parse<DefaultWaveModel>();
    MeshMap m;
    const std::string name = input.bodies.front().name;
    m[name] = two_triangles();
    const auto bodies = builder.get_bodies(m);
    const auto env = builder.get_environment_and_frames(bodies);
    ASSERT_THROW(builder.get_forces(env), SimulatorBuilderException);
}

TEST_F(SimulatorBuilderTest, can_get_forces)
{
    const double EPS = 1e-8;
    builder.can_parse<DefaultWaveModel>()
           .can_parse<GravityForceModel>()
           .can_parse<HydrostaticForceModel>();
    MeshMap m;
    const std::string name = input.bodies.front().name;
    m[name] = two_triangles();
    const auto bodies = builder.get_bodies(m);
    const auto env = builder.get_environment_and_frames(bodies);
    std::vector<ListOfForces> forces = builder.get_forces(env);
    const double cPhi   = cos(1.3);
    const double cTheta = cos(1.4);
    const double cPsi   = cos(1.5);
    const double sPhi   = sin(1.3);
    const double sTheta = sin(1.4);
    const double sPsi   = sin(1.5);
    RotationMatrix ctm;
    Eigen::Vector3d F,FF;
    ctm << cTheta*cPsi,                cTheta*sPsi,                -sTheta,
           sPhi*sTheta*cPsi-cPhi*sPsi, sPhi*sTheta*sPsi+cPhi*cPsi, sPhi*cTheta,
           cPhi*sTheta*cPsi+sPhi*sPsi, cPhi*sTheta*sPsi-sPhi*cPsi, cPhi*cTheta;
    F << 0.0, 0.0, 9.81E6;
    FF = ctm*F;
    ASSERT_EQ(1,forces.size());
    ASSERT_EQ(2,forces.front().size());
    const auto Fg = forces.front().at(0)->operator()(bodies.front());
    ASSERT_NEAR(FF(0),Fg.X(),EPS);
    ASSERT_NEAR(FF(1),Fg.Y(),EPS);
    ASSERT_NEAR(FF(2),Fg.Z(),EPS);
    ASSERT_DOUBLE_EQ(0,Fg.K());
    ASSERT_DOUBLE_EQ(0,Fg.M());
    ASSERT_DOUBLE_EQ(0,Fg.N());
}
