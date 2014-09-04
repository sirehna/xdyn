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

Wrench hydrostatic::force(const const_MeshIntersectorPtr& intersector,   //!< Mesh intersected with free surface
                          const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                          const double rho,                       //!< Density of the fluid (in kg/m^3)
                          const EPoint& g                         //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                         )
{
    // QUICK AND DIRTY HACK : choose either solution
    return fast_force(intersector,O,rho,g);
    // return exact_force(intersector,O,rho,g);
}

Wrench hydrostatic::fast_force(const const_MeshIntersectorPtr& intersector,   //!< Mesh intersected with free surface
                          const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                          const double rho,                       //!< Density of the fluid (in kg/m^3)
                          const EPoint& g                         //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                         )
{
    UnsafeWrench F(O);
    const double orientation_factor = intersector->mesh->orientation_factor;
    const double g_norm = g.norm();
    for (auto that_facet = intersector->begin_immersed() ; that_facet != intersector->end_immersed() ; ++that_facet)
    {
        const double zG = average_immersion(intersector->mesh->all_nodes, that_facet->vertex_index, intersector->all_immersions);
        const EPoint dS = orientation_factor*that_facet->area*that_facet->unit_normal;
        const EPoint ap = that_facet->barycenter;
        F += dF(O, ap , rho, g_norm, zG , dS);
    }
    return F;
}

Wrench hydrostatic::exact_force(const const_MeshIntersectorPtr& intersector,   //!< Mesh intersected with free surface
                                const Point& O,                         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                                const double rho,                       //!< Density of the fluid (in kg/m^3)
                                const EPoint& g                         //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                                )
{
    UnsafeWrench F(O);
    const double orientation_factor = intersector->mesh->orientation_factor;
    const double g_norm = g.norm();
    const EPoint down_direction = g / g_norm;
    for (auto that_facet = intersector->begin_immersed() ; that_facet != intersector->end_immersed() ; ++that_facet)
    {
        size_t facet_index = that_facet.index();
        const double zG = average_immersion(intersector->mesh->all_nodes, that_facet->vertex_index, intersector->all_immersions);
        const EPoint dS = orientation_factor*that_facet->area*that_facet->unit_normal;
        const EPoint ap = exact_application_point(Polygon(intersector->mesh,facet_index),down_direction,zG,intersector->all_immersions);
        F += dF(O, ap , rho, g_norm, zG , dS);
    }
    return F;
}

EPoint hydrostatic::normal_to_free_surface(
        const Polygon& polygon,                 //!< vertices of the facet
        const EPoint&  down_direction,           //!< local down direction expressed in mesh frame
        const std::vector<double> &all_immersions    //!< relative immersions for all point of the mesh
        )
{
    // Compute normal to free surface, oriented downward
    const Polygon free_surface = polygon.projected_on_free_surface(down_direction,all_immersions);
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
        const double  zG,                       //!< Relative immersion of facet barycentre (in metres)
        const std::vector<double> &all_immersions    //!< relative immersions for all point of the mesh
		)
{
    EPoint ns=hydrostatic::normal_to_free_surface(polygon,down_direction,all_immersions);
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
