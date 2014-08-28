/*
 * hydrostatic.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostatic.hpp"
#include "HydrostaticException.hpp"
#include "pairwise_sum.hpp"
#include "mesh_manipulations.hpp"
#include "Polygon.hpp"

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
    const int idxA = (int)idx[0];
    const double dz0 = delta_z[idx[0]];
    for (size_t i = 2 ; i < n ; ++i)
    {
        const double S = area(nodes, idxA, (int)idx[i-1], (int)idx[i]);
        areas += S;
        areas_times_points += S*(dz0+delta_z[idx[i-1]]+delta_z[idx[i]])/3.;
    }
    return areas_times_points/areas;
}

double hydrostatic::average_immersion(const std::pair<Matrix3x,std::vector<double> >& nodes //!< Coordinates of used nodes & vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
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
    if ((z[0]<0) and (z[1]>=0)) {last_was_assigned = true;}
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
                first_was_assigned = true;
            }
            if (z[i+1]>=0)
            {
                if (last_was_assigned)
                {
                    THROW(__PRETTY_FUNCTION__, HydrostaticException, "Set of emerged points is not convex.");
                }
                last = i;
                last_was_assigned = true;
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
    const bool A1_is_on_surface = v.at(idxA1)==0;
    const bool B1_is_on_surface = v.at(idxB1)==0;
    const EPoint A = M.col((int)idxA);
    const EPoint A1 = M.col((int)idxA1);
    const EPoint B = M.col((int)idxB);
    const EPoint B1 = M.col((int)idxB1);

    size_t N = (first_and_last.second>=first_and_last.first) ? n-(first_and_last.second-first_and_last.first-1) : first_and_last.second+first_and_last.first+1;
    if (A1_is_on_surface) N--;
    if (B1_is_on_surface) N--;
    Eigen::Matrix<double,3,Eigen::Dynamic> ret;
    std::vector<double> delta_z;
    ret.resize(3,(int)N);
    int k = 0;
    bool A1_inserted = false;
    bool B1_inserted = false;
    if (first_and_last.first<=first_and_last.second)
    {
        for (size_t i = 0 ; i < first_and_last.first ; ++i)
        {
            ret.col(k++) = M.col((int)idx.at(i));
            delta_z.push_back(v.at(idx.at(i)));
            A1_inserted = A1_inserted or (idx.at(i)==idxA1);
            B1_inserted = B1_inserted or (idx.at(i)==idxB1);
        }

        if (not(A1_is_on_surface))
        {
            const EPoint P = intersection(A,v.at(idxA),A1,v.at(idxA1));
            ret.col(k++) = P;
            delta_z.push_back(0);
        }
        else if (not(A1_inserted))
        {
            ret.col(k++) = M.col((int)idxA1);
            delta_z.push_back(v.at(idxA1));
        }

        if (not(B1_is_on_surface))
        {
            const EPoint Q = intersection(B,v.at(idxB),B1,v.at(idxB1));
            ret.col(k++) = Q;
            delta_z.push_back(0);
        }
        else if (not(B1_inserted))
        {
            ret.col(k++) = M.col((int)idxB1);
            delta_z.push_back(v.at(idxB1));
        }

        const size_t start = A1_is_on_surface ? first_and_last.second+2 : first_and_last.second+1;
        const size_t stop = B1_is_on_surface ? n-1 : n;
        for (size_t i = start ; i < stop ; ++i)
        {
            ret.col(k++) = M.col((int)idx.at(i));
            delta_z.push_back(v.at(idx.at(i)));
        }
    }
    else
    {
        const EPoint P = intersection(A,v.at(idxA),A1,v.at(idxA1));
        const EPoint Q = intersection(B,v.at(idxB),B1,v.at(idxB1));
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
    if (dzA*dzB>0)
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

UnsafeWrench hydrostatic::dF(const Point& O,    //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                             const EPoint& C,   //!< Point where the force is applied (barycentre of the facet)
                             const double rho,  //!< Density of the fluid (in kg/m^3)
                             const double g,    //!< Earth's standard acceleration due to gravity (eg. 9.80665 m/s^2)
                             const double z,    //!< Relative immersion (in metres)
                             const EPoint& dS   //!< Unit normal vector multiplied by the surface of the facet
                            )
{
    const EPoint F = -rho*g*z*dS; // Negative sign because the force is oriented towards the inside of the mesh but dS is oriented towards the outside of the mesh
    return UnsafeWrench(O, F, (C-O.v).cross(F));
}

Wrench hydrostatic::force(const MeshPtr& mesh,               //!< Mesh
                          const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                          const double rho,                       //!< Density of the fluid (in kg/m^3)
                          const EPoint& g,                        //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                          const std::vector<double>& immersions   //!< Relative immersion of each point in mesh (in metres)
                         )
{
    // QUICK AND DIRTY HACK : choose either solution
    return fast_force(mesh,O,rho,g,immersions);
    // return exact_force(mesh,O,rho,g,immersions);
}

Wrench hydrostatic::fast_force(const MeshPtr& mesh,               //!< Mesh
                          const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                          const double rho,                       //!< Density of the fluid (in kg/m^3)
                          const EPoint& g,                        //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                          const std::vector<double>& immersions   //!< Relative immersion of each point in mesh (in metres)
                         )
{
    if (immersions.size() != (size_t)mesh->nodes.cols())
    {
        std::stringstream ss;
        ss << "Should have as many nodes as immersions: received " << mesh->nodes.cols() << " nodes but " << immersions.size() << " immersions.";
        THROW(__PRETTY_FUNCTION__, HydrostaticException, ss.str());
    }
    std::vector<Facet>::const_iterator that_facet = mesh->facets.begin();
    UnsafeWrench F(O);
    const double orientation_factor = mesh->orientation_factor;
    const double g_norm = g.norm();
    for (size_t facet_index=0;that_facet != mesh->facets.end() ; ++that_facet , ++facet_index)
    {
        switch (get_immersion_type(that_facet->index, immersions))
        {
            case TOTALLY_IMMERGED:
            {
                const double zG = average_immersion(mesh->nodes, that_facet->index, immersions);
                const EPoint dS = that_facet->area*that_facet->unit_normal;
                const EPoint ap = fast_application_point(Polygon(mesh,facet_index));
                F += orientation_factor*dF(O, ap , rho, g_norm, zG , dS);
                break;
            }
            case PARTIALLY_EMERGED:
            {
                const std::pair<Matrix3x,std::vector<double> > polygon_and_immersions = immerged_polygon(mesh->nodes,that_facet->index,immersions);
                const double zG = average_immersion(polygon_and_immersions);
                const EPoint dS = area(polygon_and_immersions.first)*unit_normal(polygon_and_immersions.first);
                const EPoint ap = fast_application_point(Polygon(polygon_and_immersions.first));
                F += orientation_factor*dF(O,ap,rho,g_norm,zG,dS);
                break;
            }
            case TOTALLY_EMERGED:
            {
                break;
            }
        }
    }
    return F;
}

Wrench hydrostatic::exact_force(const MeshPtr& mesh,                    //!< Mesh
                                const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                                const double rho,                       //!< Density of the fluid (in kg/m^3)
                                const EPoint& g,                        //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                                const std::vector<double>& immersions   //!< Relative immersion of each point in mesh (in metres)
                                )
{
    if (immersions.size() != (size_t)mesh->nodes.cols())
    {
        std::stringstream ss;
        ss << "Should have as many nodes as immersions: received " << mesh->nodes.cols() << " nodes but " << immersions.size() << " immersions.";
        THROW(__PRETTY_FUNCTION__, HydrostaticException, ss.str());
    }
    std::vector<Facet>::const_iterator that_facet = mesh->facets.begin();
    UnsafeWrench F(O);
    const double orientation_factor = mesh->orientation_factor;
    const double g_norm = g.norm();
    const EPoint down_direction = g / g_norm;
    for (size_t facet_index=0;that_facet != mesh->facets.end() ; ++that_facet , ++facet_index)
    {
        switch (get_immersion_type(that_facet->index, immersions))
        {
            case TOTALLY_IMMERGED:
            {
                const double zG = average_immersion(mesh->nodes, that_facet->index, immersions);
                const EPoint dS = orientation_factor*that_facet->area*that_facet->unit_normal;
                const EPoint ap = exact_application_point(Polygon(mesh,facet_index),down_direction,immersions,zG);
                F += dF(O, ap , rho, g_norm, zG , dS);
                break;
            }
            case PARTIALLY_EMERGED:
            {
                const std::pair<Matrix3x,std::vector<double> > polygon_and_immersions = immerged_polygon(mesh->nodes,that_facet->index,immersions);
                const double zG = average_immersion(polygon_and_immersions);
                const EPoint dS = orientation_factor*area(polygon_and_immersions.first)*unit_normal(polygon_and_immersions.first);
                const EPoint ap = exact_application_point(Polygon(polygon_and_immersions.first),down_direction,polygon_and_immersions.second,zG);
                F += dF(O,ap,rho,g_norm,zG,dS);
                break;
            }
            case TOTALLY_EMERGED:
            {
                break;
            }
        }
    }
    return F;
}

EPoint hydrostatic::fast_application_point(
		const Polygon& polygon                //!< vertices of the facet
		)
{
	return polygon.get_barycenter();
}

EPoint hydrostatic::normal_to_free_surface(
        const Polygon& polygon,                 //!< vertices of the facet
        const EPoint&  down_direction,          //!< local down direction expressed in mesh frame
        const std::vector<double>& immersions   //!< relative immersion of each vertex
        )
{
    // Compute normal to free surface, oriented downward
    const Polygon free_surface = polygon.projected_on_free_surface(immersions,down_direction);
    const EPoint ns = free_surface.get_unit_normal();

    if(ns.norm() < 0.5 ) // the facet is vertical, we can't access the normal to free surface, but we don't need it
        return down_direction;
    if(ns.dot(down_direction) < 0) // make sure that ns is oriented downward
        return -ns;
    return ns;

}

Eigen::Matrix3d hydrostatic::facet_trihedron(
        const Polygon& polygon,     //!< vertices of the facet
        const EPoint&  ns           //!< normal to free surface, oriented downward (in mesh frame)
        )
{
    // Compute the immersion trihedron G,i2,j2,k2
    Eigen::Matrix3d R20;
    const EPoint k20 = polygon.get_unit_normal();
    EPoint i20 = ns.cross(k20);
    double tolerance = 1000*std::numeric_limits<double>::epsilon();
    if( i20.norm() < tolerance ) {
        R20.Zero();
        return R20; // quick test : facet is parallel to the free surface
    }
    i20 /= i20.norm();
    EPoint j20 = k20.cross(i20);
    if(ns.dot(j20) < 0) { // make sure that j2 is oriented downward
        i20 = -i20;
        j20 = -j20;
    }
    for(int j=0;j<3;j++){
        R20(0,j) = i20[j];
        R20(1,j) = j20[j];
        R20(2,j) = k20[j];
    }
    return R20;
}

EPoint hydrostatic::exact_application_point(
		const Polygon& polygon,                 //!< vertices of the facet
		const EPoint&  down_direction,          //!< local down direction expressed in mesh frame
		const std::vector<double>& immersions,  //!< relative immersion of each vertex
        const double  zG                        //!< Relative immersion of facet barycentre (in metres)
		)
{
    EPoint ns=hydrostatic::normal_to_free_surface(polygon,down_direction,immersions);
	Eigen::Matrix3d R20 = facet_trihedron(polygon,ns);
	if(R20.col(0).norm() < 0.5 ) // quick test : facet is parallel to the free surface
	    return polygon.get_barycenter();

	Eigen::Matrix3d JR2=polygon.get_inertia_wrt( R20 );
    Eigen::Matrix3d R02 = R20.transpose();
    EPoint j20 = R02.col(1);

	double yG = zG * ns.dot(down_direction) / (ns.dot(j20));
	double xR = JR2(0,1)/yG;
	double yR = JR2(0,0)/yG;
	EPoint offset2( xR , yR , 0);
	return polygon.get_barycenter() + R02 * offset2;
}

