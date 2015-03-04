/*
 * ClosingFacetComputerTest.cpp
 *
 *  Created on: Feb 27, 2015
 *      Author: cady
 */

#define _USE_MATH_DEFINE
#include <cmath>
#define PI M_PI

#include <Eigen/Dense>

#include "gmock/gmock.h"

#include <ssc/macros.hpp>

#include "ClosingFacetComputerTest.hpp"
#include "ClosingFacetComputer.hpp"
#include "ClosingFacetComputerException.hpp"

ClosingFacetComputerTest::ClosingFacetComputerTest() : a(ssc::random_data_generator::DataGenerator(21233))
{
}

ClosingFacetComputerTest::~ClosingFacetComputerTest()
{
}

void ClosingFacetComputerTest::SetUp()
{
}

void ClosingFacetComputerTest::TearDown()
{
}

struct TestMesh
{
    Eigen::Matrix3Xd all_nodes;
    ListOfEdges edges;
};

struct Range
{
    double min;
    double max;
};

template <typename T> struct XY
{
    XY() : x(), y() {}
    XY(const T& x_, const T& y_) : x(x_), y(y_) {}
    T x;
    T y;
};

#define X_CANVAS_SIZE 30
#define Y_CANVAS_SIZE 15
#define DOT_CHAR '*'
#define POINT_CHAR 'X'

XY<Range> get_min_max(const Eigen::Matrix3Xd M);
XY<Range> get_min_max(const Eigen::Matrix3Xd M)
{
    XY<Range> range;
    if ((M.cols()>0) and (M.rows()>0))
    {
        range.x.min = M(0,0);
        range.y.min = M(1,0);
        range.x.max = M(0,0);
        range.y.max = M(1,0);
        for (int j = 0 ; j < M.cols() ; ++j)
        {
            range.x.min = std::min(range.x.min, (double)M(0,j));
            range.x.max = std::max(range.x.max, (double)M(0,j));
            range.y.min = std::min(range.y.min, (double)M(1,j));
            range.y.max = std::max(range.y.max, (double)M(1,j));
        }
    }
    return range;
}

Eigen::Matrix3Xi convert(const Eigen::Matrix3Xd& M);
Eigen::Matrix3Xi convert(const Eigen::Matrix3Xd& M)
{
    const XY<Range> r = get_min_max(M);
    Eigen::Matrix3Xi ret = Eigen::Matrix3Xi::Zero(3, M.cols());
    for (int j = 0 ; j < M.cols() ; ++j)
    {
        ret(0,j) = (int)std::floor((M(0,j)-r.x.min)/(r.x.max-r.x.min)*X_CANVAS_SIZE+0.5);
        ret(1,j) = Y_CANVAS_SIZE - (int)std::floor((M(1,j)-r.y.min)/(r.y.max-r.y.min)*Y_CANVAS_SIZE+0.5);
    }
    return ret;
}

typedef std::vector<XY<int> > Line;

std::vector<Line> make_lines(const Eigen::Matrix3Xi& mesh, const ListOfEdges& edges);
std::vector<Line> make_lines(const Eigen::Matrix3Xi& mesh, const ListOfEdges& edges)
{
    std::vector<Line> ret;
    for (const auto edge:edges)
    {
        Line l;
        const int xmin = std::min(mesh(0,edge.first),mesh(0,edge.second));
        const int xmax = std::max(mesh(0,edge.first),mesh(0,edge.second));
        if (xmin==xmax)
        {
            const int x = xmin;
            const int ymin = std::min(mesh(1,edge.first),mesh(1,edge.second));
            const int ymax = std::max(mesh(1,edge.first),mesh(1,edge.second));
            for (int y = ymin ; y <= ymax ; ++y)
            {
                l.push_back(XY<int>(x,y));
            }
        }
        else
        {
            const double m = ((double)mesh(1,edge.first)-(double)mesh(1,edge.second))/((double)mesh(0,edge.first)-(double)mesh(0,edge.second));
            const double p = mesh(1,edge.first) - m*mesh(0,edge.first);
            for (int x = xmin ; x <= xmax ; ++x)
            {
                const int y = (int)std::floor(m*x+p + 0.5);
                l.push_back(XY<int>(x,y));
            }
        }
        ret.push_back(l);
    }
    return ret;
}

std::vector<std::string> make_mesh(const Eigen::Matrix3Xi& M, const std::vector<Line>& lines);
std::vector<std::string> make_mesh(const Eigen::Matrix3Xi& M, const std::vector<Line>& lines)
{
    const std::string spaces(X_CANVAS_SIZE+1, ' ');
    std::vector<std::string> canvas(Y_CANVAS_SIZE+1, spaces);
    for (const auto line:lines)
    {
        for (const auto dot:line)
        {
            canvas[dot.y][dot.x] = DOT_CHAR;
        }
    }
    for (int j = 0 ; j < M.cols() ; ++j)
    {
        canvas[M(1,j)][M(0,j)] = POINT_CHAR;
    }
    return canvas;
}

std::ostream& operator<<(std::ostream& os, const TestMesh& mesh);
std::ostream& operator<<(std::ostream& os, const TestMesh& mesh)
{
    const auto Mi = convert(mesh.all_nodes);
    const auto L = make_lines(Mi, mesh.edges);
    const auto lines = make_mesh(Mi, L);
    os << std::endl;
    for (const auto line:lines)
    {
        os << line << std::endl;
    }
    return os;
}

TestMesh case_0();
TestMesh case_0()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,16);
    ret.all_nodes << 0, 3, 6, 5, 6, 3, 0, 1,   3, 4, 4.5, 4,   3, 2, 1.5, 2,
                     0, 1, 0, 3, 6, 5, 6, 3, 1.5, 2,   3, 4, 4.5, 4,   3, 2,
                     0, 0, 0, 0, 0, 0, 0, 0,   0, 0,   0, 0,   0, 0,   0, 0;
    ret.edges = {{13,15},{15,9},{9,11},{11,13}};
    return ret;
}

TestMesh case_1();
TestMesh case_1()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,5);
    ret.all_nodes << 0, 1, 1, 0.5, 0,
                     0, 0, 1, 0.5, 1,
                     0, 0, 0,   0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{2,4},{4,0},{3,4}};
    return ret;
}

TestMesh case_2();
TestMesh case_2()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,5);
    ret.all_nodes << 0, 1, 1, 0.5, 0,
                     0, 0, 1, 1.5, 1,
                     0, 0, 0,   0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{2,4},{4,0},{3,4}};
    return ret;
}

TestMesh case_3();
TestMesh case_3()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << 0, 3, 3, 2, 2, 1, 1, 0,
                     0, 0, 2, 2, 1, 1, 2, 2,
                     0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0}};
    return ret;
}

TestMesh case_4();
TestMesh case_4()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 3, 2, 3, 1, 0,
                     0, 0, 1, 2, 1, 2,
                     0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{2,4},{3,5},{1,3},{4,5},{5,0},{4,0}};
    return ret;
}

TestMesh case_5();
TestMesh case_5()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 1, 2, 2, 1, 0,
                     0, 0, 0, 1, 1, 1,
                     0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{1,5},{5,0},{1,4}};
    return ret;
}

TestMesh case_6();
TestMesh case_6()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 1, 2, 2, 1, 0,
                     0, 0, 0, 2, 1, 2,
                     0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0},{1,4}};
    return ret;
}

TestMesh case_7();
TestMesh case_7()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << 0, 1, 2, 2, 1, 0, 1, 1,
                     0, 1, 0, 5, 4, 5, 2, 3,
                     0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0},{0,6},{6,2},{5,7},{7,3}};
    return ret;
}

TestMesh case_8();
TestMesh case_8()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,6);
    ret.all_nodes << 0, 1, 1, 0.5, 0, 0.5,
                     0, 0, 1,   2, 1, 0.5,
                     0, 0, 0,   0, 0,   0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,0},{3,5}};
    return ret;
}

TestMesh case_9();
TestMesh case_9()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,5);
    ret.all_nodes << 0, 1, 2, 2, 0,
                     0, 1, 0, 2, 2,
                     0, 0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,1},{1,4},{4,0}};
    return ret;
}

TestMesh case_10();
TestMesh case_10()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,16);
    ret.all_nodes << 0, 3, 6, 5, 6, 3, 0, 1,   3, 4, 4.5, 4,   3, 2, 1.5, 2,
                     0, 1, 0, 3, 6, 5, 6, 3, 1.5, 2,   3, 4, 4.5, 4,   3, 2,
                     0, 0, 0, 0, 0, 0, 0, 0,   0, 0,   0, 0,   0, 0,   0, 0;
    ret.edges = {{0,1},{1,2},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0},{0,8},{8,2},{2,10},{10,4},{4,12},{12,6},{6,14},{14,0},{8,9},{9,10},{10,11},{11,12},{12,13},{13,14},{14,15},{15,8},{1,8},{3,10},{5,12},{7,14}};
    return ret;
}

TestMesh case_11();
TestMesh case_11()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,8);
    ret.all_nodes << 2, 3, 4, 5, 4, 3, 2, 1,
                     0, 1, 2, 3, 6, 5, 4, 3,
                     0, 0, 0, 0, 0, 0, 0, 0;
    ret.edges = {{0,1},{2,3},{3,4},{4,5},{5,6},{6,7},{7,0},{5,2},{1,6}};
    return ret;
}

TestMesh case_12();
TestMesh case_12()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes << -1, 0,  0, -1,
                      1, 0, -1, -1,
                      0, 0,  0,  0;
    ret.edges = {{0,1},{1,2},{1,3}};
    return ret;
}

TestMesh case_13();
TestMesh case_13()
{
    TestMesh ret;
    ret.all_nodes = Eigen::MatrixXd::Zero(3,4);
    ret.all_nodes <<  0, 1, 1, 0,
                      0, 0, 1, 1,
                      0, 0, 0, 0;
    ret.edges = {{0,1},{1,2},{1,3}};
    return ret;
}

ClosingFacetComputer make(const TestMesh& mesh);
ClosingFacetComputer make(const TestMesh& mesh)
{
    return ClosingFacetComputer(mesh.all_nodes, mesh.edges);
}

TEST_F(ClosingFacetComputerTest, can_cluster_edges_into_independent_facets_to_compute_closing_facet)
{
    const std::vector<size_t> edges_index = {1,2,3,5,8,17};
    const std::vector<std::pair<size_t,size_t> > edges_on_free_surface = {{1,2},{2,3},{3,1},{8,10},{10,12},{12,8}};
    const std::vector<std::vector<size_t> > closing_facets=ClosingFacetComputer::group_connected_edges_into_facets(edges_index, edges_on_free_surface);
    ASSERT_EQ(2,  closing_facets.size());
    ASSERT_EQ(3,  closing_facets.at(0).size());
    ASSERT_EQ(3,  closing_facets.at(1).size());
    ASSERT_EQ(1,  closing_facets.at(0).at(0));
    ASSERT_EQ(2,  closing_facets.at(0).at(1));
    ASSERT_EQ(3,  closing_facets.at(0).at(2));
    ASSERT_EQ(5,  closing_facets.at(1).at(0));
    ASSERT_EQ(8,  closing_facets.at(1).at(1));
    ASSERT_EQ(17, closing_facets.at(1).at(2));
}

TEST_F(ClosingFacetComputerTest, can_extract_relevant_nodes)
{
    const std::vector<size_t> relevant_nodes = make(case_0()).extract_nodes();
    ASSERT_EQ(4,relevant_nodes.size());
    ASSERT_EQ(13, relevant_nodes.at(0));
    ASSERT_EQ(15, relevant_nodes.at(1));
    ASSERT_EQ(9, relevant_nodes.at(2));
    ASSERT_EQ(11, relevant_nodes.at(3));
}

TEST_F(ClosingFacetComputerTest, can_find_extreme_node)
{
    ASSERT_EQ(13, make(case_0()).find_extreme_node());
    ASSERT_EQ(0,  make(case_1()).find_extreme_node());
    ASSERT_EQ(0,  make(case_2()).find_extreme_node());
    ASSERT_EQ(0,  make(case_3()).find_extreme_node());
    ASSERT_EQ(0,  make(case_4()).find_extreme_node());
    ASSERT_EQ(0,  make(case_5()).find_extreme_node());
    ASSERT_EQ(0,  make(case_6()).find_extreme_node());
    ASSERT_EQ(0,  make(case_7()).find_extreme_node());
    ASSERT_EQ(0,  make(case_8()).find_extreme_node());
    ASSERT_EQ(0,  make(case_9()).find_extreme_node());
    ASSERT_EQ(0,  make(case_10()).find_extreme_node());
    ASSERT_EQ(7,  make(case_11()).find_extreme_node());
}

using namespace testing;

TEST_F(ClosingFacetComputerTest, can_sort_edges)
{
    ASSERT_THAT(make(case_0()).sort_edges({3,0,2,1}), ElementsAre(3, 0, 1, 2));
    ASSERT_THAT(make(case_5()).sort_edges({0,6,5}),   ElementsAre(0,5,6));
}

TEST_F(ClosingFacetComputerTest, sort_edges_should_throw_if_not_all_edges_are_connected)
{
    ASSERT_THROW(make(case_0()).sort_edges({1,3}),ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, sort_edges_should_throw_if_some_indexes_are_out_of_range)
{
    ASSERT_THROW(make(case_0()).sort_edges({1,8}),ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, can_retrieve_angle_between_two_edges)
{
    ASSERT_DOUBLE_EQ(PI/2, make(case_1()).edge_angle(0,1));
    ASSERT_DOUBLE_EQ(3*PI/4-PI/2, make(case_1()).edge_angle(1,2));
    ASSERT_DOUBLE_EQ(-PI/2, make(case_1()).edge_angle(2,5));
}

TEST_F(ClosingFacetComputerTest, angle_between_closing_edges_should_be_the_greatest_when_positive)
{
    ASSERT_GT(make(case_13()).edge_angle(0,1), make(case_13()).edge_angle(0,2));
    ASSERT_GT(make(case_10()).edge_angle(3,4), make(case_10()).edge_angle(3,12));
}

TEST_F(ClosingFacetComputerTest, angle_between_closing_edges_should_be_the_lowest_when_negative)
{
    ASSERT_LT(make(case_12()).edge_angle(0,1), make(case_12()).edge_angle(0,2));
}

TEST_F(ClosingFacetComputerTest, edge_angle_should_throw_if_edges_do_not_exist)
{
    ASSERT_THROW(make(case_1()).edge_angle(5,6), ClosingFacetComputerException);
    ASSERT_THROW(make(case_1()).edge_angle(6,5), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, edge_angle_should_throw_if_edges_do_not_have_a_point_in_common)
{
    ASSERT_THROW(make(case_1()).edge_angle(0,3), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, can_find_edges_connected_to_second_point_of_a_particular_edge)
{
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(0), ElementsAre(1));
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(1), ElementsAre(2,3));
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(3), ElementsAre(4,5));
    ASSERT_THAT(make(case_1()).edges_connected_to_second_node_of_edge(5), ElementsAre(3,4));
    ASSERT_THAT(make(case_9()).edges_connected_to_second_node_of_edge(3), ElementsAre(0,1,4));
}

TEST_F(ClosingFacetComputerTest, next_edge_should_throw_if_edge_does_not_exist)
{
    ASSERT_THROW(make(case_1()).next_edge(10), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, next_edge_should_throw_if_edge_is_not_connected_to_anything)
{
    ASSERT_THROW(make(case_8()).next_edge(5), ClosingFacetComputerException);
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_1)
{
    ASSERT_EQ(1, make(case_1()).next_edge(0));
    ASSERT_EQ(3, make(case_1()).next_edge(1));
    ASSERT_EQ(4, make(case_1()).next_edge(3));
    ASSERT_EQ(0, make(case_1()).next_edge(4));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_2)
{
    ASSERT_EQ(1, make(case_2()).next_edge(0));
    ASSERT_EQ(2, make(case_2()).next_edge(1));
    ASSERT_EQ(5, make(case_2()).next_edge(2));
    ASSERT_EQ(4, make(case_2()).next_edge(5));
    ASSERT_EQ(0, make(case_2()).next_edge(4));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_3)
{
    ASSERT_EQ(1, make(case_3()).next_edge(0));
    ASSERT_EQ(2, make(case_3()).next_edge(1));
    ASSERT_EQ(3, make(case_3()).next_edge(2));
    ASSERT_EQ(4, make(case_3()).next_edge(3));
    ASSERT_EQ(5, make(case_3()).next_edge(4));
    ASSERT_EQ(6, make(case_3()).next_edge(5));
    ASSERT_EQ(7, make(case_3()).next_edge(6));
    ASSERT_EQ(0, make(case_3()).next_edge(7));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_4)
{
    ASSERT_EQ(5, make(case_4()).next_edge(0));
    ASSERT_EQ(4, make(case_4()).next_edge(5));
    ASSERT_EQ(7, make(case_4()).next_edge(4));
    ASSERT_EQ(0, make(case_4()).next_edge(7));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_5)
{
    ASSERT_EQ(1, make(case_5()).next_edge(0));
    ASSERT_EQ(2, make(case_5()).next_edge(1));
    ASSERT_EQ(3, make(case_5()).next_edge(2));
    ASSERT_EQ(4, make(case_5()).next_edge(3));
    ASSERT_EQ(6, make(case_5()).next_edge(4));
    ASSERT_EQ(0, make(case_5()).next_edge(6));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_6)
{
    ASSERT_EQ(1, make(case_6()).next_edge(0));
    ASSERT_EQ(2, make(case_6()).next_edge(1));
    ASSERT_EQ(3, make(case_6()).next_edge(2));
    ASSERT_EQ(4, make(case_6()).next_edge(3));
    ASSERT_EQ(5, make(case_6()).next_edge(4));
    ASSERT_EQ(0, make(case_6()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_7)
{
    ASSERT_EQ(1, make(case_7()).next_edge(0));
    ASSERT_EQ(2, make(case_7()).next_edge(1));
    ASSERT_EQ(3, make(case_7()).next_edge(2));
    ASSERT_EQ(4, make(case_7()).next_edge(3));
    ASSERT_EQ(5, make(case_7()).next_edge(4));
    ASSERT_EQ(0, make(case_7()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_8)
{
    ASSERT_EQ(1, make(case_8()).next_edge(0));
    ASSERT_EQ(2, make(case_8()).next_edge(1));
    ASSERT_EQ(3, make(case_8()).next_edge(2));
    ASSERT_EQ(4, make(case_8()).next_edge(3));
    ASSERT_EQ(0, make(case_8()).next_edge(4));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_9)
{
    ASSERT_EQ(1, make(case_9()).next_edge(0));
    ASSERT_EQ(2, make(case_9()).next_edge(1));
    ASSERT_EQ(3, make(case_9()).next_edge(2));
    ASSERT_EQ(4, make(case_9()).next_edge(3));
    ASSERT_EQ(5, make(case_9()).next_edge(4));
    ASSERT_EQ(0, make(case_9()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_10)
{
    ASSERT_EQ(1, make(case_10()).next_edge(0));
    ASSERT_EQ(2, make(case_10()).next_edge(1));
    ASSERT_EQ(3, make(case_10()).next_edge(2));
    ASSERT_EQ(4, make(case_10()).next_edge(3));
    ASSERT_EQ(5, make(case_10()).next_edge(4));
    ASSERT_EQ(6, make(case_10()).next_edge(5));
    ASSERT_EQ(7, make(case_10()).next_edge(6));
    ASSERT_EQ(0, make(case_10()).next_edge(7));
}

TEST_F(ClosingFacetComputerTest, can_find_the_next_edge_in_a_contour_for_case_11)
{
    ASSERT_EQ(8, make(case_11()).next_edge(0));
    ASSERT_EQ(2, make(case_11()).next_edge(1));
    ASSERT_EQ(3, make(case_11()).next_edge(2));
    ASSERT_EQ(4, make(case_11()).next_edge(3));
    ASSERT_EQ(5, make(case_11()).next_edge(4));
    ASSERT_EQ(6, make(case_11()).next_edge(5));
}

TEST_F(ClosingFacetComputerTest, can_find_first_extreme_edge)
{
    ASSERT_EQ(0, make(case_1()).first_extreme_edge());
    ASSERT_EQ(0, make(case_2()).first_extreme_edge());
    ASSERT_EQ(0, make(case_3()).first_extreme_edge());
    ASSERT_EQ(0, make(case_4()).first_extreme_edge());
    ASSERT_EQ(0, make(case_5()).first_extreme_edge());
    ASSERT_EQ(0, make(case_6()).first_extreme_edge());
    ASSERT_EQ(5, make(case_7()).first_extreme_edge());
    ASSERT_EQ(0, make(case_8()).first_extreme_edge());
    ASSERT_EQ(5, make(case_9()).first_extreme_edge());
    ASSERT_EQ(7, make(case_10()).first_extreme_edge());
    ASSERT_EQ(0, make(case_11()).first_extreme_edge());
}
