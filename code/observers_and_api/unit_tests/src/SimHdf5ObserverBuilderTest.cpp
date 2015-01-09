#include "H5Cpp.h"
#include "Sim.hpp"
#include "SimHdf5ObserverBuilder.hpp"
#include "SimHdf5ObserverBuilderTest.hpp"

SimHdf5ObserverBuilderTest::SimHdf5ObserverBuilderTest() : a(ssc::random_data_generator::DataGenerator(666))
{
}

SimHdf5ObserverBuilderTest::~SimHdf5ObserverBuilderTest()
{
}

void SimHdf5ObserverBuilderTest::SetUp()
{
}

void SimHdf5ObserverBuilderTest::TearDown()
{
}

TEST_F(SimHdf5ObserverBuilderTest, should_be_able_to_create_hdf5_quaternion_type)
{
    H5_CreateIdQuaternion();
}

TEST_F(SimHdf5ObserverBuilderTest, should_be_able_to_create_hdf5_euler_angle_type)
{
    H5_CreateIdEulerAngle();
}

TEST_F(SimHdf5ObserverBuilderTest, should_be_able_to_create_hdf5_wrench_type)
{
    H5_CreateIdWrenchType();
}

TEST_F(SimHdf5ObserverBuilderTest, should_be_able_to_create_hdf5_states_type)
{
    VectorOfStringModelForEachBody v;
    for (size_t i = 0 ; i < 3 ; ++i)
        v.push_back(std::make_pair(a.random<std::string>(),std::vector<std::string>()));
    H5_CreateIdStates(v);
}

TEST_F(SimHdf5ObserverBuilderTest, should_be_able_to_create_hdf5_efforts_type)
{
    VectorOfStringModelForEachBody v;
    const size_t n = a.random<size_t>().between(3,10);
    for (size_t i = 0 ; i < 3 ; ++i)
        v.push_back(std::make_pair(a.random<std::string>(),a.random_vector_of<std::string>().of_size(n)));
    H5_CreateIdEfforts(v);
}
