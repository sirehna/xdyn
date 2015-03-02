/*
 * ClosingFacetComputerTest.cpp
 *
 *  Created on: Feb 27, 2015
 *      Author: cady
 */

#include <Eigen/Dense>

#include <ssc/macros.hpp>

#include "ClosingFacetComputerTest.hpp"
#include "ClosingFacetComputer.hpp"

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

