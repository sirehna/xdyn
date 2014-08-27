#ifndef POLYGON_HPP_
#define POLYGON_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)
class Mesh;
typedef TR1(shared_ptr)<Mesh> MeshPtr;

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

    private:
        Polygon(); // Disabled
};

#endif /* POLYGON_HPP_ */
