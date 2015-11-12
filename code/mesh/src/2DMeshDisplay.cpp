/*
 * 2DMeshDisplay.cpp
 *
 *  Created on: Mar 4, 2015
 *      Author: cady
 */


#include "2DMeshDisplay.hpp"

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

Eigen::Matrix3Xi convert(const Eigen::Matrix3Xd& M)
{
    const XY<Range> r = get_min_max(M);
    const double delta_x = r.x.max == r.x.min ? 1 : r.x.max-r.x.min;
    const double delta_y = r.y.max == r.y.min ? 1 : r.y.max-r.y.min;
    Eigen::Matrix3Xi ret = Eigen::Matrix3Xi::Zero(3, M.cols());
    for (int j = 0 ; j < M.cols() ; ++j)
    {
        ret(0,j) = (int)std::floor((M(0,j)-r.x.min)/delta_x*X_CANVAS_SIZE+0.5);
        ret(1,j) = Y_CANVAS_SIZE - (int)std::floor((M(1,j)-r.y.min)/delta_y*Y_CANVAS_SIZE+0.5);
    }
    return ret;
}

std::vector<Line> make_lines(const Eigen::Matrix3Xi& mesh, const ClosingFacetComputer::ListOfEdges& edges)
{
    std::vector<Line> ret;
    for (const auto edge:edges)
    {
        Line l;
        const int xmin = std::min(mesh(0,(long)edge.first),mesh(0,(long)edge.second));
        const int xmax = std::max(mesh(0,(long)edge.first),mesh(0,(long)edge.second));
        if (xmin==xmax)
        {
            const int x = xmin;
            const int ymin = std::min(mesh(1,(long)edge.first),mesh(1,(long)edge.second));
            const int ymax = std::max(mesh(1,(long)edge.first),mesh(1,(long)edge.second));
            for (int y = ymin ; y <= ymax ; ++y)
            {
                l.push_back(XY<int>(x,y));
            }
        }
        else
        {
            const double m = ((double)mesh(1,(long)edge.first)-(double)mesh(1,(long)edge.second))/((double)mesh(0,(long)edge.first)-(double)mesh(0,(long)edge.second));
            const double p = mesh(1,(long)edge.first) - m*mesh(0,(long)edge.first);
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

std::vector<std::string> make_mesh(const Eigen::Matrix3Xi& M, const std::vector<Line>& lines, const std::set<size_t>& idx_to_display)
{
    const std::string spaces(X_CANVAS_SIZE+1, ' ');
    std::vector<std::string> canvas(Y_CANVAS_SIZE+1, spaces);
    for (const auto line:lines)
    {
        for (const auto dot:line)
        {
            canvas[(size_t)dot.y][(size_t)dot.x] = DOT_CHAR;
        }
    }
    for (const auto j:idx_to_display)
    {
        std::stringstream ss;
        ss << j;
        canvas[(unsigned long)M(1,(long)j)].replace((unsigned long)M(0,(long)j), ss.str().size(), ss.str());
    }
    return canvas;
}

std::ostream& operator<<(std::ostream& os, const TestMesh& mesh)
{
    const auto Mi = convert(mesh.all_nodes);
    const auto L = make_lines(Mi, mesh.edges);
    std::set<size_t> nodes_to_display;
    for (const auto edge:mesh.edges)
    {
        nodes_to_display.insert(edge.first);
        nodes_to_display.insert(edge.second);
    }
    const auto lines = make_mesh(Mi, L, nodes_to_display);
    os << std::endl;
    for (const auto line:lines)
    {
        os << line << std::endl;
    }
    return os;
}
