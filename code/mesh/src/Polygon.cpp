#include "mesh_manipulations.hpp"
#include "MeshBuilder.hpp"
#include "Polygon.hpp"

Polygon::Polygon(const MeshPtr& mesh_, const size_t facet_idx_) : mesh(mesh_), facet_idx(facet_idx_),
area(mesh->facets.at(facet_idx).area)
{
}

Polygon::Polygon(const Matrix3x& matrix //!< Mesh given as a matrix (one point per column, all columns are used)
                ) : mesh(MeshPtr(new Mesh(MeshBuilder(matrix).build()))), facet_idx(0), area(::area(matrix))
{
}


/**  \returns Area of the polygon
  *  \snippet mesh/unit_tests/src/PolygonTest.cpp PolygonTest example
  */
double Polygon::get_area() const
{
    return area;
}
