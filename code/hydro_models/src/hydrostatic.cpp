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

ImmersionStatus hydrostatic::get_immersion_type(const std::vector<size_t>& idx, const std::vector<double>& delta_z)
{
    const size_t n = idx.size();
    bool some_points_are_immerged = false;
    bool some_points_are_emerged = false;
    for (size_t i = 0 ; i < n ; ++i)
    {
        const double dz = delta_z.at(idx[i]);
        if (dz > 0) some_points_are_immerged = true;
        if (dz < 0) some_points_are_emerged = true;
    }
    if (some_points_are_immerged and not(some_points_are_emerged)) return TOTALLY_IMMERGED;
    if (not(some_points_are_immerged))                             return TOTALLY_EMERGED;
                                                                   return PARTIALLY_EMERGED;
}

double hydrostatic::average_immersion(const Matrix3x& nodes,             //!< Coordinates of all nodes
                                      const std::vector<size_t>& idx,    //!< Indices of the points
                                      const std::vector<double>& delta_z //!< Vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                            )
{
    const size_t n = idx.size();
    double areas_times_points = 0;
    double areas = 0;
    const int idxA = idx[0];
    for (size_t i = 2 ; i < n ; ++i)
    {
        const double S = area(nodes, idxA, (int)idx[i-1], (int)idx[i]);
        areas += S;
        areas_times_points += S*(delta_z[idx[0]]+delta_z[idx[i-1]]+delta_z[idx[i]])/3.;
    }
    return areas_times_points/areas;
}

double hydrostatic::average_immersion(const std::pair<Matrix3x,std::vector<double> >& nodes             //!< Coordinates of used nodes & vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                                     )
{
    const size_t n = (size_t)nodes.first.cols();
    if (n != nodes.second.size())
    {
        std::stringstream ss;
        ss << "Mesh contains " << n << " nodes but delta_z contains " << nodes.second.size() << " values: they should be the same size.";
        THROW(__PRETTY_FUNCTION__, HydrostaticException, ss.str());
    }
    std::vector<size_t> idx;
    for (size_t i = 0 ; i < n ; ++i) idx.push_back(i);
    return average_immersion(nodes.first, idx, nodes.second);
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
    switch(get_immersion_type(idx, v))
    {
        case TOTALLY_EMERGED:
        {
            THROW(__PRETTY_FUNCTION__, HydrostaticException, "None of the points are immerged.");
            break;
        }
        case TOTALLY_IMMERGED:
        {
            THROW(__PRETTY_FUNCTION__, HydrostaticException, "All the points are immerged.");
            break;
        }
        default:
            break;
    }
}

std::pair<Matrix3x,std::vector<double> > hydrostatic::immerged_polygon(const Matrix3x& M,
                                       const std::vector<size_t>& idx,
                                       const std::vector<double>& v)
{
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
    const EPoint A = M.col((int)idxA);
    const EPoint A1 = M.col((int)idxA1);
    const EPoint B = M.col((int)idxB);
    const EPoint B1 = M.col((int)idxB1);
    const EPoint P = intersection(A,v.at(idxA),A1,v.at(idxA1));
    const EPoint Q = intersection(B,v.at(idxB),B1,v.at(idxB1));
    const size_t N = (first_and_last.second>=first_and_last.first) ? n-(first_and_last.second-first_and_last.first-1) : first_and_last.second+first_and_last.first+1;
    Eigen::Matrix<double,3,Eigen::Dynamic> ret;
    std::vector<double> delta_z;
    ret.resize(3,(int)N);
    int k = 0;
    if (first_and_last.first<=first_and_last.second)
    {
        for (size_t i = 0 ; i < first_and_last.first ; ++i)
        {
            ret.col(k++) = M.col((int)idx.at(i));
            delta_z.push_back(v.at(idx.at(i)));
        }
        ret.col(k++) = P;
        delta_z.push_back(0);
        ret.col(k++) = Q;
        delta_z.push_back(0);
        for (size_t i = first_and_last.second+1 ; i < n ; ++i)
        {
            ret.col(k++) = M.col((int)idx.at(i));
            delta_z.push_back(v.at(idx.at(i)));
        }
    }
    else
    {
        ret.col(k++) = Q;
        delta_z.push_back(0);
        for (size_t i = idxB1 ; i <= idxA1 ; ++i)
        {
            ret.col(k++) = M.col((int)i);
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
    const EPoint F = -rho*g*z*dS; // Negative sign because the force is oriented towards the inside of the mesh but dS is oriented towards the outside of the mesh
    return UnsafeWrench(O, F, (C-O.v).cross(F));
}

UnsafeWrench hydrostatic::dF(const Point& O,           //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                             const Facet& f,           //!< Point where the force is applied (barycentre of the facet)
                             const double rho,         //!< Density of the fluid (in kg/m^3)
                             const double g,           //!< Earth's standard acceleration due to gravity (eg. 9.80665 m/s^2)
                             const double immersion    //!< Relative immersion of the barycentre (in metres)
                       )
{
    const EPoint F = -rho*g*immersion*f.area*f.unit_normal; // Negative sign because the force is oriented towards the inside of the mesh but dS is oriented towards the outside of the mesh
    return UnsafeWrench(O, F, (f.barycenter-O.v).cross(F));
}

Wrench hydrostatic::force(const Mesh& mesh,                       //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                          const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                          const double rho,                       //!< Density of the fluid (in kg/m^3)
                          const double g,                         //!< Earth's standard acceleration due to gravity (eg. 9.80665 m/s^2)
                          const std::vector<double>& immersions   //!< Relative immersion of each point in mesh (in metres)
                )
{
    if (immersions.size() != (size_t)mesh.nodes.cols())
    {
        std::stringstream ss;
        ss << "Should have as many nodes as immersions: received " << mesh.nodes.cols() << " nodes but " << immersions.size() << " immersions.";
        THROW(__PRETTY_FUNCTION__, HydrostaticException, ss.str());
    }
    std::vector<Facet>::const_iterator that_facet = mesh.facets.begin();
    UnsafeWrench F(O);
    for (;that_facet != mesh.facets.end() ; ++that_facet)
    {
        switch (get_immersion_type(that_facet->index, immersions))
        {
            case TOTALLY_IMMERGED:
            {
                const double zG = average_immersion(mesh.nodes, that_facet->index, immersions);
                F += dF(O, *that_facet, rho, g, zG);
                break;
            }
            case PARTIALLY_EMERGED:
            {
                const std::pair<Matrix3x,std::vector<double> > polygon_and_immersions = immerged_polygon(mesh.nodes,that_facet->index,immersions);
                const double zG = average_immersion(polygon_and_immersions);
                const EPoint dS = area(polygon_and_immersions.first)*unit_normal(polygon_and_immersions.first);
                F += dF(O,barycenter(polygon_and_immersions.first),rho,g,zG,dS);
                break;
            }
            case TOTALLY_EMERGED:
            {
                break;
            }
        }
    }
    return (Wrench)F;
}
