#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include <cstdlib> //For size_t

#include "tr1_macros.hpp"
#include TR1INC(memory)
class Mesh;
typedef TR1(shared_ptr)<Mesh> MeshPtr;

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

    private:
        Polygon(); // Disabled
        MeshPtr mesh;
        size_t facet_idx;
        double area;
        EPoint unit_normal;
};

#endif /* POLYGON_HPP_ */
