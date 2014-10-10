/*
 * OutputGeneratorTest.cpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#include "OutputTransformerTest.hpp"
#include "OutputTransformer.hpp"
#include "yaml_data.hpp"
#include "simulator_api.hpp"
#include <ssc/solver.hpp>
#include "SimulatorYamlParser.hpp"

#include <ssc/macros.hpp>

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#define EPS (1E-8)

#include <ssc/exception_handling.hpp>

#define N 40

class OutputTransformerTestException: public Exception
{
    public:
        OutputTransformerTestException(const char* s) :
                Exception(s)
        {
        }
};

std::vector<std::map<std::string,double> > OutputTransformerTest::get_results(const std::string& yaml, const std::string& mesh_file)
{
    std::vector<std::map<std::string,double> > ret;
    YamlSimulatorInput parsed_yaml = SimulatorYamlParser(yaml).parse();
    parsed_yaml.bodies.front().mesh = mesh_file;
    auto res = simulate<ssc::solver::EulerStepper>(parsed_yaml, 0, N, 1);
    const OutputTransformer transform(parsed_yaml);
    for (auto r=res.begin() ; r != res.end() ; ++r) ret.push_back(transform(*r));
    return ret;
}

const std::vector<std::map<std::string,double> > OutputTransformerTest::falling_ball = OutputTransformerTest::get_results(test_data::falling_ball_example()+test_data::outputs());
const std::vector<std::map<std::string,double> > OutputTransformerTest::full_example = OutputTransformerTest::get_results(test_data::full_example());
const std::vector<std::map<std::string,double> > OutputTransformerTest::rolling_cube = OutputTransformerTest::get_results(test_data::rolling_cube());
const std::vector<std::map<std::string,double> > OutputTransformerTest::falling_cube = OutputTransformerTest::get_results(test_data::falling_cube());

double OutputTransformerTest::get(const std::vector<std::map<std::string,double> >& results, const size_t body_idx, const std::string& variable_name) const
{
    const auto it = results.at(body_idx).find(variable_name);
    if (it == results.at(body_idx).end())
    {
        THROW(__PRETTY_FUNCTION__, OutputTransformerTestException, std::string("Unable to find variable '" + variable_name + "'"));
    }
    return it->second;
}

OutputTransformerTest::OutputTransformerTest() :
                a(ssc::random_data_generator::DataGenerator(42022))
{
}

OutputTransformerTest::~OutputTransformerTest()
{
}

void OutputTransformerTest::SetUp()
{
}

void OutputTransformerTest::TearDown()
{
}

TEST_F(OutputTransformerTest, processed_output_should_have_the_right_size)
{
    ASSERT_EQ(N+1, falling_ball.size());
    ASSERT_EQ(N+1, full_example.size());
    ASSERT_EQ(N+1, rolling_cube.size());
    ASSERT_EQ(N+1, falling_cube.size());
}

TEST_F(OutputTransformerTest, position_outputs_should_exist_in_map)
{
    ASSERT_NO_THROW(get(falling_ball, 0, "x(O in NED / ball -> ball)"));
    ASSERT_NO_THROW(get(falling_ball, 0, "z(O in NED / ball -> ball)"));
}

TEST_F(OutputTransformerTest, angle_outputs_should_exist_in_map)
{
    ASSERT_NO_THROW(get(full_example, 0, "phi(body 1 / NED -> body 1)"));
    ASSERT_NO_THROW(get(full_example, 0, "theta(body 1 / NED -> body 1)"));
    ASSERT_NO_THROW(get(full_example, 0, "psi(body 1 / NED -> body 1)"));
}

TEST_F(OutputTransformerTest, can_compute_positions)
{
    const auto x0 = get(falling_ball, 0, "x(O in NED / ball -> ball)");
    const auto z0 = get(falling_ball, 0, "z(O in NED / ball -> ball)");
    const auto x2 = get(falling_ball, N, "x(O in NED / ball -> ball)");;
    const auto z2 = get(falling_ball, N, "z(O in NED / ball -> ball)");
    ASSERT_DOUBLE_EQ(-4,         x0) << "t = 0";
    ASSERT_DOUBLE_EQ(-12,        z0) << "t = 0";
    ASSERT_NEAR(-(4+N),     x2, EPS) << "t = 2";
    ASSERT_NEAR(-(12+0.5*9.81*N*(N-1)), z2, EPS) << "t = 2";
}

TEST_F(OutputTransformerTest, can_compute_angles)
{
    const auto phi   = get(full_example, 0, "phi(body 1 / NED -> body 1)");
    const auto theta = get(full_example, 0, "theta(body 1 / NED -> body 1)");
    const auto psi   = get(full_example, 0, "psi(body 1 / NED -> body 1)");
    EXPECT_NEAR(1.3, phi, EPS);
    EXPECT_NEAR(1.4, theta, EPS);
    EXPECT_NEAR(1.5, psi, EPS);
}

TEST_F(OutputTransformerTest, output_should_contain_kinetic_energy)
{
    ASSERT_NO_THROW(get(falling_ball, 0, "Ec(ball)"));
    ASSERT_NO_THROW(get(full_example, 0, "Ec(body 1)"));
    ASSERT_NO_THROW(get(rolling_cube, 0, "Ec(cube)"));
    ASSERT_NO_THROW(get(falling_cube, 0, "Ec(cube)"));
}

TEST_F(OutputTransformerTest, output_should_contain_potential_energy)
{
    ASSERT_NO_THROW(get(falling_ball, 0, "Ep(ball)"));
    ASSERT_NO_THROW(get(full_example, 0, "Ep(body 1)"));
    ASSERT_NO_THROW(get(rolling_cube, 0, "Ep(cube)"));
    ASSERT_NO_THROW(get(falling_cube, 0, "Ep(cube)"));
}

TEST_F(OutputTransformerTest, output_should_contain_mechanical_energy)
{
    ASSERT_NO_THROW(get(falling_ball, 0, "Em(ball)"));
    ASSERT_NO_THROW(get(full_example, 0, "Em(body 1)"));
    ASSERT_NO_THROW(get(rolling_cube, 0, "Em(cube)"));
    ASSERT_NO_THROW(get(falling_cube, 0, "Em(cube)"));
}

TEST_F(OutputTransformerTest, mechanical_energy_should_be_the_sum_of_potential_plus_kinetic_energy)
{
    for (size_t i = 0 ; i < N ; ++i)
    {
        const auto Ec_falling_ball = get(falling_ball, i, "Ec(ball)");
        const auto Ep_falling_ball = get(falling_ball, i, "Ep(ball)");
        const auto Em_falling_ball = get(falling_ball, i, "Em(ball)");
        ASSERT_DOUBLE_EQ(Em_falling_ball, Ec_falling_ball+Ep_falling_ball);

        const auto Ec_falling_cube = get(falling_cube, i, "Ec(cube)");
        const auto Ep_falling_cube = get(falling_cube, i, "Ep(cube)");
        const auto Em_falling_cube = get(falling_cube, i, "Em(cube)");
        ASSERT_DOUBLE_EQ(Em_falling_cube, Ec_falling_cube+Ep_falling_cube);

        const auto Ec_rolling_cube = get(rolling_cube, i, "Ec(cube)");
        const auto Ep_rolling_cube = get(rolling_cube, i, "Ep(cube)");
        const auto Em_rolling_cube = get(rolling_cube, i, "Em(cube)");
        ASSERT_DOUBLE_EQ(Em_rolling_cube, Ec_rolling_cube+Ep_rolling_cube);
    }
}

TEST_F(OutputTransformerTest, can_retrieve_kinetic_energy_of_falling_ball)
{
    ASSERT_NEAR(1E6/2, get(falling_ball, 0, "Ec(ball)"), EPS);
    for (size_t i = 1 ; i < N ; ++i)
    {
        const double m = 1E6;
        const double u = 1;
        const double w = 9.81*i;
        EXPECT_SMALL_RELATIVE_ERROR(0.5*m*(u*u+w*w), get(falling_ball, i, "Ec(ball)"), EPS) << " (i = " << i << ")";
    }
}
