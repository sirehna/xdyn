#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <cstdlib> //For size_t

#include "Mesh.hpp"

#include "GeometricTypes3d.hpp"

/** \brief Created to treat operations on emerged & partially immersed facets homogeneously.
 *  \details When implementing the hydrostatic force's exact point of application
 *  (i.e. not the centre of gravity), it became apparent that an object was missing
 *  to avoid duplicating the code in hydrostatic::dF: This class fills this gap
 *  and gives a common way to access a facet's area, unit normal & barycenter
 *  whether the facet comes from a Mesh object or whether it is a Matrix3x (eg.
 *  when computing the immersed polygon).
 *  \addtogroup mesh
 *  \ingroup mesh
 *  \section ex1 Example
 *  \snippet mesh/unit_tests/src/PolygonTest.cpp PolygonTest example
 *  \section ex2 Expected output
 *  \snippet mesh/unit_tests/src/PolygonTest.cpp PolygonTest expected output
 */
class Polygon
{
    public:
        Polygon(const Matrix3x& mesh //!< Mesh given as a matrix (one point per column, all columns are used)
                );
        Polygon(const MeshPtr& mesh,   //!< Points & facets (not all points are used)
                const size_t facet_idx //!< Index of the facet used to define the polygon
                );

        /**  \returns Area of the polygon
          *  \snippet mesh/unit_tests/src/PolygonTest.cpp PolygonTest example
          */
        double get_area() const;

        /**  \returns Unit normal to the the polygon
          *  \snippet mesh/unit_tests/src/PolygonTest.cpp PolygonTest unit_normal_example
          */
        EPoint get_unit_normal() const;

        /**  \returns Barycenter of the polygon
          *  \snippet mesh/unit_tests/src/PolygonTest.cpp PolygonTest unit_normal_example
          */
        EPoint get_barycenter() const;

		/**  \details Compute the inertia matrix of the polygon w.r.t; provided inertia frame R2;
		  *  the inertia frame is specified thru a coordinate transform matrix versus mesh frame R0 (from mesh frame R0 to inertia frame R2);
		  *  assume that first 2 axis of inertia frame are parallel to the facet, and that 3rd axis is orthogonal to the facet
          */
        Eigen::Matrix3d get_inertia_wrt(
        			const Eigen::Matrix3d R20 //!< coordinates of inertia frame vectors versus mesh frame
        		) const;

		/**  \details Compute the projection of a polygon on free surface knowing vertical direction and immersions of each vertex
		  *  the inertia frame is specified thru a coordinate transform matrix versus mesh frame R0 (from mesh frame R0 to inertia frame R2);
		  *  assume that first 2 axis of inertia frame are parallel to the facet, and that 3rd axis is orthogonal to the facet
          */
        Polygon projected_on_free_surface(
        		const std::vector<double>& immersions    ,  //!< relative immersion of each vertex
        		const EPoint&              down_direction   //!< local down direction expressed in mesh frame
        ) const;
    private:
        Polygon(); // Disabled
        MeshPtr mesh;
        size_t facet_idx;
        double area;
        EPoint unit_normal;
        EPoint barycenter;
};

#endif /* POLYGON_HPP_ */
