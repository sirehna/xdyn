#include "mesh_manipulations.hpp"
#include "MeshBuilder.hpp"
#include "Polygon.hpp"

Polygon::Polygon(const MeshPtr& mesh_, const size_t facet_idx_) : mesh(mesh_), facet_idx(facet_idx_),
area(mesh->facets.at(facet_idx).area), unit_normal(mesh->facets.at(facet_idx).unit_normal), barycenter(mesh->facets.at(facet_idx).barycenter)
{
}

Polygon::Polygon(const Matrix3x& matrix //!< Mesh given as a matrix (one point per column, all columns are used)
                ) : mesh(MeshPtr(new Mesh(MeshBuilder(matrix).build()))), facet_idx(0), area(::area(matrix)), unit_normal(::unit_normal(matrix)), barycenter(::barycenter(matrix))
{
}

double Polygon::get_area() const
{
    return area;
}

EPoint Polygon::get_unit_normal() const
{
    return unit_normal;
}

EPoint Polygon::get_barycenter() const
{
    return barycenter;
}
