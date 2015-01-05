/*
 * hydrostatic.cpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#include "hydrostatic.hpp"
#include <ssc/numeric.hpp>
#include "mesh_manipulations.hpp"

#include <algorithm> // std::count_if
#include <iterator>  // std::distance

using namespace hydrostatic;

double hydrostatic::average_immersion(const std::vector<size_t>& idx,    //!< Indices of the points
                                      const std::vector<double>& delta_z //!< Vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                                      )
{
    const size_t n = idx.size();
    double average = 0;
    for (size_t i = 0 ; i < n ; ++i)
    {
        average += delta_z[idx[i]];
    }
    if (n) average /= (double)n;
    return average;
}

EPoint hydrostatic::normal_to_free_surface(
            const FacetIterator&       that_facet,      //!< the facet of interest
            const EPoint&              down_direction,  //!< local down direction expressed in mesh frame
            const Matrix3x&            all_nodes,       //!< the nodes of the mesh
            const std::vector<double>& all_immersions   //!< the immersions for all nodes of the mesh
            )
{
    // Compute normal to free surface, oriented downward
    const Matrix3x vertices = project_facet_on_free_surface(that_facet,down_direction,all_nodes,all_immersions);
    const EPoint ns = ::unit_normal(vertices);

    if(ns.norm() < 0.5 ) // the facet is vertical, we can't access the normal to free surface, but we don't need it
        return down_direction;
    if(ns.dot(down_direction) < 0) // make sure that ns is oriented downward
        return -ns;
    return ns;
}

Eigen::Matrix3d hydrostatic::facet_trihedron(
        const EPoint&  n ,  //!< the normal to the facet
        const EPoint&  ns   //!< the normal to free surface, oriented downward (in mesh frame)
        )
{
    // Compute the immersion trihedron G,i2,j2,k2
    Eigen::Matrix3d R20;
    const EPoint k20 = n;
    EPoint i20 = ns.cross(k20);
    const double tolerance = 1000*std::numeric_limits<double>::epsilon();
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
            const FacetIterator&       that_facet,     //!< the facet of interest
            const EPoint&              down_direction, //!< local down direction expressed in mesh frame
            const double               zG,             //!< Relative immersion of facet barycentre (in metres)
            const Matrix3x&            all_nodes,      //!< the nodes of the mesh
            const std::vector<double>& all_immersions  //!< the immersions for all nodes of the mesh
            )
{
    const EPoint n=that_facet->unit_normal;
    const EPoint ns=normal_to_free_surface(that_facet,down_direction,all_nodes,all_immersions);
	const Eigen::Matrix3d R20 = facet_trihedron(n,ns);
	if(R20.col(0).norm() < 0.5 ) // quick test : facet is parallel to the free surface
	    return that_facet->barycenter;

	const Eigen::Matrix3d JR2=get_inertia_of_polygon_wrt(that_facet,R20,all_nodes );
    const Eigen::Matrix3d R02 = R20.transpose();
    const EPoint j20 = R02.col(1);

	const double yG = zG * ns.dot(down_direction) / (ns.dot(j20));
	const double xR = JR2(0,1)/yG;
	const double yR = JR2(0,0)/yG;
	const EPoint offset2( xR , yR , 0);
	return that_facet->barycenter + R02 * offset2;
}

Matrix3x hydrostatic::project_facet_on_free_surface(
            const FacetIterator&       that_facet,     //!< the facet of interest
            const EPoint&              down_direction, //!< local down direction expressed in mesh frame
            const Matrix3x&            all_nodes,      //!< the nodes of the mesh
            const std::vector<double>& all_immersions  //!< the immersions for all nodes of the mesh
            )
{
    const size_t nVertices = that_facet->vertex_index.size();
    Matrix3x newVertices(3,nVertices);
    for(size_t iVertex = 0 ; iVertex < nVertices ; ++ iVertex) {
        size_t vertex_index = that_facet->vertex_index[iVertex];
        newVertices.col((int)iVertex) = all_nodes.col((int)vertex_index) - all_immersions[vertex_index]*down_direction;
    }
    return newVertices;
}

Eigen::Matrix3d hydrostatic::get_inertia_of_polygon_wrt(
            const FacetIterator&  that_facet,  //!< the facet of interest
            const Eigen::Matrix3d R20,         //!< coordinates of inertia frame vectors versus mesh frame
            const Matrix3x&       all_nodes    //!< the nodes of the mesh
            )
{
    // Compute the coordinates of facet vertices in R2
    const EPoint G=that_facet->barycenter;
    const size_t nVertices = that_facet->vertex_index.size();
    Matrix3x verticesInR0(3,nVertices);
    for(size_t iVertex = 0 ; iVertex < nVertices ; ++ iVertex) {
        verticesInR0.col((int)iVertex) = all_nodes.col((int)that_facet->vertex_index[iVertex]) - G;
    }
    Matrix3x verticesInR2(3,nVertices);
    verticesInR2 = R20 * verticesInR0;

    return inertia_of_polygon(verticesInR2);
}
