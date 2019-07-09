#include "ConfBuilderTest.hpp"
#include "yaml_data.hpp"
#include "ConfBuilder.hpp"
#include "TriMeshTestData.hpp"


ConfBuilderTest::ConfBuilderTest() : a(ssc::random_data_generator::DataGenerator(78910))
{
}

ConfBuilderTest::~ConfBuilderTest()
{
}

void ConfBuilderTest::SetUp()
{
}

void ConfBuilderTest::TearDown()
{
}

TEST_F(ConfBuilderTest, compute_right_value_of_Tmax_falling_ball)
{
    ConfBuilder builder(test_data::falling_ball_example());
    ASSERT_EQ(builder.Tmax, 0);
}

TEST_F(ConfBuilderTest, can_build_with_mesh)
{
    ConfBuilder builder(test_data::falling_ball_example(), unit_cube());
    ASSERT_FALSE(builder.sim.get_bodies().empty());
    ASSERT_GT(builder.sim.get_bodies().front()->get_states().mesh.use_count(), 0);
    ASSERT_GT(builder.sim.get_bodies().front()->get_states().mesh->nb_of_static_edges, 0);
}
