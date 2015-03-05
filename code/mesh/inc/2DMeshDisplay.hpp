/*
 * 2DMeshDisplay.hpp
 *
 *  Created on: Mar 4, 2015
 *      Author: cady
 */


#ifndef DMESHDISPLAY_HPP_
#define DMESHDISPLAY_HPP_

#include "ClosingFacetComputer.hpp"

struct TestMesh
{
    Eigen::Matrix3Xd all_nodes;
    ClosingFacetComputer::ListOfEdges edges;
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
Eigen::Matrix3Xi convert(const Eigen::Matrix3Xd& M);
typedef std::vector<XY<int> > Line;
std::vector<Line> make_lines(const Eigen::Matrix3Xi& mesh, const ClosingFacetComputer::ListOfEdges& edges);
std::vector<std::string> make_mesh(const Eigen::Matrix3Xi& M, const std::vector<Line>& lines, const std::set<size_t>& idx_to_display);
std::ostream& operator<<(std::ostream& os, const TestMesh& mesh);

#endif  /* DMESHDISPLAY_HPP_ */
