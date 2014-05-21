/*
 * hydrostatic.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */


#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"
#include "kahan_sum.hpp"
#include "pairwise_sum.hpp"
#include "mesh_manipulations.hpp"

#include <algorithm> // std::count_if
#include <iterator>  // std::distance

using namespace hydrostatic;

size_t get_nb_of_immerged_points(const std::vector<size_t>& idx, const std::vector<double>& delta_z);
size_t get_nb_of_immerged_points(const std::vector<size_t>& idx, const std::vector<double>& delta_z)
{
    const size_t n = idx.size();
    size_t nb_of_immerged_points = 0;
    for (size_t i = 0 ; i < n ; ++i)
    {
        if (delta_z.at(idx[i]) >= 0) nb_of_immerged_points++;
    }
    return nb_of_immerged_points;
}

double hydrostatic::average_immersion(const Matrix3x& nodes,             //!< Coordinates of all nodes
                                      const std::vector<size_t>& idx,    //!< Indices of the points
                                      const std::vector<double>& delta_z //!< Vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                            )
{
    const size_t n = idx.size();
    std::vector<EPoint> areas_times_points;
    std::vector<double> areas;
    const size_t iA = idx[0];
    for (size_t i = 2 ; i < n ; ++i)
    {
        const size_t iB = idx[i-1];
        const size_t iC = idx[i];
        const EPoint A = nodes.col(iA);
        const EPoint B = nodes.col(iB);
        const EPoint C = nodes.col(iC);
        const EPoint centre_of_gravity((A(0)+B(0)+C(0))/3.,
                                                (A(1)+B(1)+C(1))/3.,
                                                (delta_z.at(idx[0])+delta_z.at(idx[i-1])+delta_z.at(idx[i]))/3.);
        areas.push_back(triangle_area(A, B, C));
        areas_times_points.push_back(areas.back()*centre_of_gravity);
    }
    return (sum::pairwise(areas_times_points)/sum::pairwise(areas))(2,0);
}

double hydrostatic::average_immersion(const Matrix3x& nodes,             //!< Coordinates of used nodes
                                      const std::vector<double>& delta_z //!< Vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                                     )
{
    const size_t n = nodes.cols();
    if (n != delta_z.size())
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "Number of nodes should be equal to size of delta_z");
    }
    std::vector<size_t> idx;
    for (size_t i = 0 ; i < n ; ++i) idx.push_back(i);
    return average_immersion(nodes, idx, delta_z);
}

std::pair<size_t,size_t> hydrostatic::first_and_last_emerged_points(const std::vector<double>& z)
{
    const size_t n = z.size();
    if (n<3)
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "Need at least 3 points in z");
    }
    size_t first = 0;
    size_t last = 0;
    bool first_was_assigned = false;
    bool last_was_assigned = false;
    if ((z[0]<0) and (z[1]>=0)) {last_was_assigned=true;}
    for (size_t i = 1 ; i < n-1 ; ++i)
    {
        if (z[i]<0)
        {
            if (z[i-1]>=0)
            {
                if (first_was_assigned)
                {
                    THROW(__PRETTY_FUNCTION__, HydrostaticException, "Set of emerged points is not convex.");
                }
                first = i;
                first_was_assigned=true;
            }
            if (z[i+1]>=0)
            {
                if (last_was_assigned)
                {
                    THROW(__PRETTY_FUNCTION__, HydrostaticException, "Set of emerged points is not convex.");
                }
                last = i;
                last_was_assigned=true;
            }
        }
    }
    if ((z[n-1]<0) and not(first_was_assigned)) first = n-1;
    if ((z[n-1]<0) and not(last_was_assigned))  last = n-1;
    return std::make_pair(first,last);
}

void make_sure_some_points_are_immerged_and_some_are_not(const std::vector<size_t>& idx, const std::vector<double>& v);
void make_sure_some_points_are_immerged_and_some_are_not(const std::vector<size_t>& idx, const std::vector<double>& v)
{
    const size_t n = idx.size();
    const size_t number_of_immerged_nodes = get_nb_of_immerged_points(idx, v);

    if (number_of_immerged_nodes == 0)
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "None of the points are immerged.");
    }
    if (number_of_immerged_nodes == n)
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "All the points are immerged.");
    }
}

std::pair<Matrix3x,std::vector<double> > hydrostatic::immerged_polygon(const Matrix3x& M,
                                       const std::vector<size_t>& idx,
                                       const std::vector<double>& v)
{
    make_sure_some_points_are_immerged_and_some_are_not(idx, v);
    const size_t n = idx.size();
    std::vector<double> dz(n,0);
    for (size_t i = 0 ; i < n ; ++i)
    {
        dz[i] = v.at(idx[i]);
    }
    const std::pair<size_t,size_t> first_and_last = first_and_last_emerged_points(dz);
    const size_t idxA = idx[first_and_last.first];
    const size_t idxB = idx[first_and_last.second];
    const size_t idxA1 = previous(idx, idxA);
    const size_t idxB1 = next(idx, idxB);
    const EPoint A = M.col(idxA);
    const EPoint A1 = M.col(idxA1);
    const EPoint B = M.col(idxB);
    const EPoint B1 = M.col(idxB1);
    const EPoint P = intersection(A,v[idxA],A1,v[idxA1]);
    const EPoint Q = intersection(B,v[idxB],B1,v[idxB1]);
    const size_t N = (idxB>=idxA) ? n-(idxB-idxA-1) : idxA-idxB+2;
    Eigen::Matrix<double,3,Eigen::Dynamic> ret;
    std::vector<double> delta_z;
    ret.resize(3,N);
    size_t k = 0;

    if (idxA<idxB)
    {
        for (size_t i = 0 ; i < idxA ; ++i)
        {
            ret.col(k++) = M.col(i);
            delta_z.push_back(v.at(i));
        }
        ret.col(k++) = P;
        delta_z.push_back(0);
        ret.col(k++) = Q;
        delta_z.push_back(0);
        for (size_t i = idxB+1 ; i < n ; ++i)
        {
            ret.col(k++) = M.col(i);
            delta_z.push_back(v.at(i));
        }
    }
    else
    {
        ret.col(k++) = Q;
        delta_z.push_back(0);
        for (size_t i = idxB1 ; i <= idxA1 ; ++i)
        {
            ret.col(k++) = M.col(i);
            delta_z.push_back(v.at(i));
        }
        ret.col(k++) = P;
        delta_z.push_back(0);
    }
    return std::make_pair(ret,delta_z);
}

EPoint hydrostatic::intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB)
{
    if (dzA*dzB>=0)
    {
        THROW(__PRETTY_FUNCTION__, HydrostaticException, "zB & zA must have different signs");
    }
    const double xA = A(0);
    const double xB = B(0);
    const double yA = A(1);
    const double yB = B(1);
    const double zA = A(2);
    const double zB = B(2);
    const double k = dzA/(dzA-dzB);
    return EPoint(xA + k*(xB-xA),
                  yA + k*(yB-yA),
                  zA + k*(zB-zA)
                  );

}

size_t hydrostatic::next(const std::vector<size_t>& idx, const size_t i0)
{
    const size_t n = idx.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        if (i0 == idx[i])
        {
            if (i== n-1) return idx[0];
            else         return idx[i+1];
        }
    }
    std::stringstream ss;
    ss << "Unable to find index " << i0 << " in list.";
    THROW(__PRETTY_FUNCTION__, HydrostaticException, ss.str());
    return 0;
}

size_t hydrostatic::previous(const std::vector<size_t>& idx, const size_t i0)
{
    const size_t n = idx.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        if (i0 == idx[i])
        {
            if (i== 0) return idx[n-1];
            else       return idx[i-1];
        }
    }
    std::stringstream ss;
    ss << "Unable to find index " << i0 << " in list.";
    THROW(__PRETTY_FUNCTION__, HydrostaticException, ss.str());
    return 0;
}

UnsafeWrench hydrostatic::dF(const Point& O,           //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                             const EPoint& C, //!< Point where the force is applied (barycentre of the facet)
                             const double rho,         //!< Density of the fluid (in kg/m^3)
                             const double g,           //!< Earth's standard acceleration due to gravity (eg. 9.80665 m/s^2)
                             const double z,           //!< Relative immersion (in metres)
                             const EPoint& dS //!< Unit normal vector multiplied by the surface of the facet
                           )
{
    const EPoint F = rho*g*z*dS;
    return UnsafeWrench(O.get_frame(), F, (C-O.v).cross(F));
}
