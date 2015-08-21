/*
 * BlockedDOFTest.cpp
 *
 *  Created on: Aug 19, 2015
 *      Author: cady
 */


#include <fstream>

#include <boost/filesystem.hpp> // For boost::filesystem::unique_path

#include "BlockedDOF.hpp"
#include "BlockedDOFException.hpp"
#include "BlockedDOFTest.hpp"
#include "gmock/gmock.h"
using namespace testing; // So we can use 'ElementsAre' unqualified
#include "SimulatorYamlParserException.hpp"

BlockedDOFTest::BlockedDOFTest() : a(ssc::random_data_generator::DataGenerator(854512))
{
}

BlockedDOFTest::~BlockedDOFTest()
{
}

void BlockedDOFTest::SetUp()
{
}

void BlockedDOFTest::TearDown()
{
}

TEST_F(BlockedDOFTest, can_parse_forced_dof)
{
    const std::string yaml = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";
    const BlockedDOF::Yaml input = BlockedDOF::parse(yaml);
    ASSERT_EQ(1, input.from_yaml.size());
    ASSERT_EQ(BlockedDOF::BlockableState::P, input.from_yaml.front().state);
    ASSERT_EQ(BlockedDOF::InterpolationType::PIECEWISE_CONSTANT, input.from_yaml.front().interpolation);
    ASSERT_THAT(input.from_yaml.front().t, ElementsAre(4.2));
    ASSERT_THAT(input.from_yaml.front().value, ElementsAre(5));
    ASSERT_EQ(1, input.from_csv.size());
    ASSERT_EQ(BlockedDOF::BlockableState::U, input.from_csv.front().state);
    ASSERT_EQ(BlockedDOF::InterpolationType::SPLINE, input.from_csv.front().interpolation);
    ASSERT_EQ("T", input.from_csv.front().t);
    ASSERT_EQ("PS", input.from_csv.front().value);
    ASSERT_EQ("test.csv", input.from_csv.front().filename);
}

TEST_F(BlockedDOFTest, should_throw_if_forcing_anything_other_than_uvwpqr)
{
    const std::string yaml = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: x\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";
    ASSERT_THROW(BlockedDOF::parse(yaml), SimulatorYamlParserException);
}

TEST_F(BlockedDOFTest, should_throw_if_forcing_same_state_twice)
{
    const std::string yaml1 = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n";
    const std::string yaml2 =
                             "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n"
                             "     - state: p\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";
    const std::string yaml3 = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n"
                             "   from YAML:\n"
                             "     - state: u\n"
                             "       t: [4.2]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";

    ASSERT_THROW(BlockedDOF b(yaml1), BlockedDOFException);
    ASSERT_THROW(BlockedDOF b(yaml2), BlockedDOFException);
    ASSERT_THROW(BlockedDOF b(yaml3), BlockedDOFException);
}

TEST_F(BlockedDOFTest, should_throw_if_not_as_many_values_as_instants)
{
    const std::string invalid_yaml =
                             "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [4.2,5]\n"
                             "       value: [5]\n"
                             "       interpolation: piecewise constant\n";
    ASSERT_THROW(BlockedDOF b(invalid_yaml), BlockedDOFException);
}

TEST_F(BlockedDOFTest, should_throw_if_t_not_strictly_increasing)
{
    const std::string invalid_yaml =
                             "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [5,4.2]\n"
                             "       value: [5,6]\n"
                             "       interpolation: piecewise constant\n";
    ASSERT_THROW(BlockedDOF b(invalid_yaml), BlockedDOFException);
}

TEST_F(BlockedDOFTest, interpolation_type_should_be_valid)
{
    const std::string invalid_yaml =
                                 "blocked dof:\n"
                                 "   from YAML:\n"
                                 "     - state: p\n"
                                 "       t: [5,4.2]\n"
                                 "       value: [5,6]\n"
                                 "       interpolation: something\n";
    ASSERT_THROW(BlockedDOF::parse(invalid_yaml), SimulatorYamlParserException);
}

TEST_F(BlockedDOFTest, should_throw_if_CSV_file_does_not_exist)
{
    const std::string yaml = "blocked dof:\n"
                             "   from CSV:\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: test.csv\n";
    ASSERT_THROW(BlockedDOF b(yaml), BlockedDOFException);
}

struct TmpFile
{
    TmpFile() : path(boost::filesystem::unique_path())
    {
    }

    ~TmpFile()
    {
        if (boost::filesystem::exists(path)) boost::filesystem::remove(path);
    }

    TmpFile& operator<<(const std::string& s)
    {
        std::ofstream of(path.string(), std::ofstream::out | std::ofstream::app);
        of << s;
        of.close();
        return *this;
    }

    std::string get_filename() const
    {
        return path.string();
    }

    private:
        TmpFile(const TmpFile& rhs);
        TmpFile& operator=(const TmpFile& rhs);
        boost::filesystem::path path;
};


TEST_F(BlockedDOFTest, should_not_throw_if_CSV_file_exists)
{
    TmpFile csv_file;
    const std::string yaml = "blocked dof"
                             "   from CSV:\n"
                             "     - state: u\n"
                             "       t: T\n"
                             "       value: PS\n"
                             "       interpolation: spline\n"
                             "       filename: " + csv_file.get_filename();
    const std::string csv = "T,PS\n"
                            "1,2\n";
    csv_file << csv;
    ASSERT_NO_THROW(BlockedDOF b(yaml));
}

TEST_F(BlockedDOFTest, piecewise_constant_should_work)
{
    const std::string yaml = "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: p\n"
                             "       t: [1,4.2,5]\n"
                             "       value: [1,2,3]\n"
                             "       interpolation: piecewise constant\n";
    StateType x(13);
    const BlockedDOF blocker(yaml);
    blocker.force_states(x,1);
    ASSERT_DOUBLE_EQ(1, x[PIDX(0)]);
    blocker.force_states(x,2.6);
    ASSERT_DOUBLE_EQ(1, x[PIDX(0)]);
    blocker.force_states(x,4.2);
    ASSERT_DOUBLE_EQ(2, x[PIDX(0)]);
    blocker.force_states(x,4.6);
    ASSERT_DOUBLE_EQ(2, x[PIDX(0)]);
    blocker.force_states(x,5);
    ASSERT_DOUBLE_EQ(3, x[PIDX(0)]);
}

TEST_F(BlockedDOFTest, linear_should_work)
{
    const std::string yaml = "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: u\n"
                             "       t: [1,4.2,5]\n"
                             "       value: [1,2,3]\n"
                             "       interpolation: linear\n";
    StateType x(13);
    const BlockedDOF blocker(yaml);
    blocker.force_states(x,1);
    ASSERT_DOUBLE_EQ(1, x[UIDX(0)]);
    blocker.force_states(x,2.6);
    ASSERT_DOUBLE_EQ(1.5, x[UIDX(0)]);
    blocker.force_states(x,4.2);
    ASSERT_DOUBLE_EQ(2, x[UIDX(0)]);
    blocker.force_states(x,4.6);
    ASSERT_DOUBLE_EQ(2.5, x[UIDX(0)]);
    blocker.force_states(x,5);
    ASSERT_DOUBLE_EQ(3, x[UIDX(0)]);
}

TEST_F(BlockedDOFTest, spline_should_work)
{
    const std::string yaml = "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: q\n"
                             "       t: [1,4.2,5]\n"
                             "       value: [1,2,3]\n"
                             "       interpolation: spline\n";
    StateType x(13);
    const BlockedDOF blocker(yaml);
    blocker.force_states(x,1);
    ASSERT_DOUBLE_EQ(1, x[QIDX(0)]);
    blocker.force_states(x,2.6);
    ASSERT_NEAR(0.9,    x[QIDX(0)], 1e-6);
    blocker.force_states(x,4.2);
    ASSERT_DOUBLE_EQ(2, x[QIDX(0)]);
    blocker.force_states(x,4.6);
    ASSERT_NEAR(2.4625, x[QIDX(0)], 1e-6);
    blocker.force_states(x,5);
    ASSERT_DOUBLE_EQ(3, x[QIDX(0)]);
}

TEST_F(BlockedDOFTest, blocked_derivative_should_work)
{
    const std::string yaml = "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: u\n"
                             "       t: [1,4.2,5]\n"
                             "       value: [1,2,3]\n"
                             "       interpolation: linear\n";
    StateType dx_dt(13);
    const BlockedDOF blocker(yaml);
    blocker.force_state_derivatives(dx_dt,1);
    ASSERT_DOUBLE_EQ(1./3.2, dx_dt[UIDX(0)]);
    blocker.force_state_derivatives(dx_dt,2.6);
    ASSERT_DOUBLE_EQ(1./3.2, dx_dt[UIDX(0)]);
    blocker.force_state_derivatives(dx_dt,4.2);
    ASSERT_DOUBLE_EQ(1./0.8, dx_dt[UIDX(0)]);
    blocker.force_state_derivatives(dx_dt,4.6);
    ASSERT_DOUBLE_EQ(1./0.8, dx_dt[UIDX(0)]);
    blocker.force_state_derivatives(dx_dt,5);
    ASSERT_DOUBLE_EQ(1./0.8, dx_dt[UIDX(0)]);
}

TEST_F(BlockedDOFTest, force_delta)
{
    const std::string yaml = "blocked dof:\n"
                             "   from YAML:\n"
                             "     - state: u\n"
                             "       t: [1,4.2,5]\n"
                             "       value: [1,2,3]\n"
                             "       interpolation: linear\n";
    const StateType dx_dt = {10,11,12,13,14,15,16,17,18,19,20,21,23};
    const BlockedDOF blocker(yaml);
    Eigen::Matrix<double,6,6> total_inertia;
    total_inertia << 1, 2, 3, 4, 5, 6,
                     7, 8, 9, 0, 1, 2,
                     3, 4, 5, 6, 7, 8,
                     9, 0, 1, 2, 3, 4,
                     5, 6, 7, 8, 9, 0,
                     1, 2, 3, 4, 5, 6;
    Eigen::Vector3d F,T;
    F(0) = 50;
    F(1) = 51;
    F(2) = 52;
    T(0) = 60;
    T(1) = 61;
    T(2) = 62;
    ssc::kinematics::Wrench sum_of_other_forces(ssc::kinematics::Point("d",1,2,3),F,T);
    const auto delta_F = blocker.get_delta_F(dx_dt,total_inertia,sum_of_other_forces);
    EXPECT_DOUBLE_EQ(293, (double)delta_F(0));
    EXPECT_DOUBLE_EQ(340, (double)delta_F(1));
    EXPECT_DOUBLE_EQ(477, (double)delta_F(2));
    EXPECT_DOUBLE_EQ(227, (double)delta_F(3));
    EXPECT_DOUBLE_EQ(474, (double)delta_F(4));
    EXPECT_DOUBLE_EQ(281, (double)delta_F(5));
}
