/*
 * hydrostatic.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATIC_HPP_
#define HYDROSTATIC_HPP_

#include <vector>
#include <cstddef> //size_t
#include <utility> //std::pair
#include <Eigen/Dense>

#include "GeometricTypes3d.hpp"
#include "MeshIntersector.hpp"
#include <ssc/kinematics.hpp>

class Polygon;

namespace hydrostatic
{
    /**  \author cec
      *  \date Apr 30, 2014, 10:10:40 AM
      *  \brief Computes average relative immersion of three points
      *  \details Needs the indices of those points & a vector of relative heights.
      *  \returns Average relative immersion
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest average_immersion_example
      */
    double average_immersion(const std::vector<size_t>& idx,     //!< Indices of the points
                             const std::vector<double>& delta_z  //!< Vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                            );

    /**  \author cec
      *  \date May 21, 2014, 12:27:34 PM
      *  \brief
      *  \details Nodes should have the same number of columns as the size of delta_z
      *  \returns
      */
    double average_immersion(const std::vector<double>& nodes //!< Coordinates of used nodes & vector of relative wave heights (in metres) of all nodes (positive if point is immerged)
                            );


    /**  \author cec
      *  \date May 19, 2014, 10:30:33 AM
      *  \brief Hydrostatic force & torque acting on a single facet of the mesh
      *  \details Points O & C are assumed to be in the same frame of reference, which is
      *           also the frame of reference of the calculated Wrench object.
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest dF_example
      */
    ssc::kinematics::UnsafeWrench dF(const ssc::kinematics::Point& O,         //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                                     const EPoint& C,        //!< Point where the force is applied (barycentre of the facet)
                                     const double rho,       //!< Density of the fluid (in kg/m^3)
                                     const double g,         //!< Earth's standard acceleration due to gravity (eg. 9.80665 m/s^2)
                                     const double immersion, //!< Relative immersion of the barycentre (in metres)
                                     const EPoint& dS        //!< Unit normal vector multiplied by the surface of the facet
        );

    /**  \author cec
      *  \date May 19, 2014, 3:19:19 PM
      *  \brief Computes the hydrostatic force acting on a body
      *  \details
      *  \returns
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest force_example
      */
    ssc::kinematics::Wrench force(const const_MeshIntersectorPtr& intersector, //!< Mesh intersected with free surface
                                  const ssc::kinematics::Point& O,             //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                                  const double rho,                            //!< Density of the fluid (in kg/m^3)
                                  const EPoint& g                              //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
        );

    /**  \brief Computes the hydrostatic force acting on a body (fast but inexact)
      *  \details
      *  \returns
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest force_example
      */
    ssc::kinematics::Wrench fast_force(const const_MeshIntersectorPtr& intersector, //!< Mesh intersected with free surface
                                       const ssc::kinematics::Point& O,             //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                                       const double rho,                            //!< Density of the fluid (in kg/m^3)
                                       const EPoint& g                              //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                                      );

    /**  \brief Computes the hydrostatic force acting on a body applied at exact application point
      *  \details
      *  \returns
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest force_example
      */
    ssc::kinematics::Wrench exact_force(const const_MeshIntersectorPtr& intersector, //!< Mesh intersected with free surface
                                        const ssc::kinematics::Point& O,             //!< Point at which the Wrench will be given (eg. the body's centre of gravity)
                                        const double rho,                            //!< Density of the fluid (in kg/m^3)
                                        const EPoint& g                              //!< Earth's standard acceleration vector due to gravity (eg. 9.80665 m/s^2) (in the body's mesh frame)
                                        );


    /** \details Compute normal to free surface, oriented downward, knowing the facet vertex, immersion of each vertex and down direction (all in mesh frame)
      *  If ever the facet is vertical, this function doesn't have access to normal of free surface, but down_direction can be used instead
      *  \see Hydrostatic Force on a plane Surface p. 61-64, Ref ???
      */
    EPoint normal_to_free_surface(
            const FacetIterator&       that_facet,      //!< the facet of interest
            const EPoint&              down_direction,  //!< local down direction expressed in mesh frame
            const Matrix3x&            all_nodes,       //!< the nodes of the mesh
            const std::vector<double>& all_immersions   //!< the immersions for all nodes of the mesh
        );

    /** \details Compute a trihedron R2 of the facet convenient for computation of hydrostatic application point:
      * - first axis i2 is parallel to the facet and to the free surface
      * - second axis j2 is parallel to the facet and oriented downward
      * - third axis k2 is normal to the facet
      *  \returns the coordinate transform matrix of R2 w.r.t. the mesh frame R0 (I.O.W. from R0 to R2), or a null matrix if facet is parallel to free surface
      *  \see Hydrostatic Force on a plane Surface p. 61-64, Ref ???
      */
    Eigen::Matrix3d facet_trihedron(
            const EPoint&  n ,  //!< the normal to the facet
            const EPoint&  ns   //!< the normal to free surface, oriented downward (in mesh frame)
        );

    /**  \brief Compute the application point of hydrostatic force for a triangular facet, slowly but exactly
      * \ details
      *  \returns application point coordinates expressed in same frame as barycentre
      *  \see Hydrostatic Force on a plane Surface p. 61-64, Ref ???
      */
    EPoint exact_application_point(
            const FacetIterator&       that_facet,     //!< the facet of interest
            const EPoint&              down_direction, //!< local down direction expressed in mesh frame
            const double               zG,             //!< Relative immersion of facet barycentre (in metres)
            const Matrix3x&            all_nodes,      //!< the nodes of the mesh
            const std::vector<double>& all_immersions  //!< the immersions for all nodes of the mesh
        );

    /**  \details Compute the projection of a facet on free surface knowing vertical direction and immersions of each vertex
      */
    Matrix3x project_facet_on_free_surface(
            const FacetIterator&       that_facet,     //!< the facet of interest
            const EPoint&              down_direction, //!< local down direction expressed in mesh frame
            const Matrix3x&            all_nodes,      //!< the nodes of the mesh
            const std::vector<double>& all_immersions  //!< the immersions for all nodes of the mesh
        );

    /**  \details Compute the inertia matrix of the facet w.r.t. provided inertia frame R2;
      *  the inertia frame is specified thru a coordinate transform matrix versus mesh frame R0 (from mesh frame R0 to inertia frame R2);
      *  assume that first 2 axis of inertia frame are parallel to the facet, and that 3rd axis is orthogonal to the facet
      */
    Eigen::Matrix3d get_inertia_of_polygon_wrt(
            const FacetIterator&  that_facet,  //!< the facet of interest
            const Eigen::Matrix3d R20,         //!< coordinates of inertia frame vectors versus mesh frame
            const Matrix3x&       all_nodes    //!< the nodes of the mesh
        );

}

#endif /* HYDROSTATIC_HPP_ */
