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

Eigen::Matrix3d Polygon::get_inertia_wrt(
			const Eigen::Matrix3d R20 //!< coordinates of inertia frame vectors versus mesh frame
		) const
{
	// Compute the coordinates of facet vertices in R2
	EPoint G=get_barycenter();
	size_t nVertices = mesh->facets[facet_idx].index.size();
	Matrix3x verticesInR0(3,nVertices);
	for(size_t iVertex = 0 ; iVertex < nVertices ; ++ iVertex) {
		verticesInR0.col((int)iVertex) = mesh->nodes.col((int)mesh->facets[facet_idx].index[iVertex]) - G;
	}
	Matrix3x verticesInR2(3,nVertices);
	verticesInR2 = R20 * verticesInR0;

	return inertia_of_polygon( verticesInR2 );
}

Polygon Polygon::projected_on_free_surface(
		const std::vector<double>& immersions    ,  //!< relative immersion of each vertex
		const EPoint&              down_direction   //!< local down direction expressed in mesh frame
		) const
{
	size_t nVertices = mesh->facets[facet_idx].index.size();
	Matrix3x newVertices(3,nVertices);
	for(size_t iVertex = 0 ; iVertex < nVertices ; ++ iVertex) {
		newVertices.col((int)iVertex) = mesh->nodes.col((int)mesh->facets[facet_idx].index[iVertex]) - immersions[iVertex]*down_direction;
	}
	return Polygon(newVertices);
}
