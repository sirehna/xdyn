#include <cmath>
#include <algorithm>

#include "InternalErrorException.hpp"
#include "MeshException.hpp"
#include "Mesh.hpp"
#include "MeshBuilderTest.hpp"
#include "TriMeshTestData.hpp"
#include "MeshBuilder.hpp"

size_t get_oriented_edge_index(const size_t &e)
{
    return e >> 1;
}

MeshBuilderTest::MeshBuilderTest()
{
}

TEST_F(MeshBuilderTest, should_be_able_to_build_a_mesh_from_a_single_triangle)
{
    const Mesh m = MeshBuilder(one_triangle()).build();
    ASSERT_EQ(1,m.nb_of_static_facets);
    ASSERT_EQ(3,m.nb_of_static_nodes);
    ASSERT_EQ(3,m.nb_of_static_edges);
}

TEST_F(MeshBuilderTest, adding_twice_a_triangle_should_not_change_the_number_of_nodes_nor_edges)
{
    VectorOfVectorOfPoints triangles;
    triangles.push_back(one_triangle());
    triangles.push_back(one_triangle());
    Mesh m = MeshBuilder(triangles).build();
    ASSERT_EQ(2,m.nb_of_static_facets);
    ASSERT_EQ(3,m.nb_of_static_nodes);
    ASSERT_EQ(3,m.nb_of_static_edges);
}

TEST_F(MeshBuilderTest, should_be_able_to_evaluate_the_barycenter_of_a_single_triangle)
{
    const Mesh m = MeshBuilder(one_triangle()).build();
    ASSERT_EQ(1,m.nb_of_static_facets);
    ASSERT_EQ(3,m.nb_of_static_nodes);
    ASSERT_DOUBLE_EQ((1.0+2.1+3.1)/3.0,(double)m.facets.at(0).centre_of_gravity(0));
    ASSERT_DOUBLE_EQ((2.1+3.7+4.5)/3.0,(double)m.facets.at(0).centre_of_gravity(1));
    ASSERT_DOUBLE_EQ((3.2+4.5+6.7)/3.0,(double)m.facets.at(0).centre_of_gravity(2));
}

TEST_F(MeshBuilderTest, should_be_able_to_evaluate_the_area_of_a_single_triangle)
{
    const Mesh m = MeshBuilder(one_triangle()).build();
    Eigen::Vector3d n1,n2,v;
    ASSERT_EQ(1,m.nb_of_static_facets);
    ASSERT_EQ(3,m.nb_of_static_nodes);
    n1(0)= 2.1-1.0;
    n1(1)= 3.7-2.1;
    n1(2)= 4.5-3.2;
    n2(0)= 3.1-1.0;
    n2(1)= 4.5-2.1;
    n2(2)= 6.7-3.2;
    v(0) = n1(1)*n2(2)-n1(2)*n2(1);
    v(1) = n1(2)*n2(0)-n1(0)*n2(2);
    v(2) = n1(0)*n2(1)-n1(1)*n2(0);
    ASSERT_DOUBLE_EQ(0.5*sqrt(v(0)*v(0)+v(1)*v(1)+v(2)*v(2)),(double)m.facets.at(0).area);
}

TEST_F(MeshBuilderTest, should_be_able_to_evaluate_the_normal_of_a_single_triangle)
{
    const Mesh m = MeshBuilder(one_triangle()).build();

    ASSERT_EQ(1,m.nb_of_static_facets);
    ASSERT_EQ(3,m.nb_of_static_nodes);
    ASSERT_DOUBLE_EQ(+2.48/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),(double)m.facets.at(0).unit_normal(0));
    ASSERT_DOUBLE_EQ(-1.12/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),(double)m.facets.at(0).unit_normal(1));
    ASSERT_DOUBLE_EQ(-0.72/sqrt(2.48*2.48+1.12*1.12+0.72*0.72),(double)m.facets.at(0).unit_normal(2));
}

TEST_F(MeshBuilderTest, should_be_able_to_represent_a_cube)
{
    const Mesh m = MeshBuilder(unit_cube()).build();
    ASSERT_EQ(12,m.nb_of_static_facets);
    ASSERT_EQ(8,m.nb_of_static_nodes);
    ASSERT_EQ(18,m.nb_of_static_edges);
    for (size_t i=0;i<12;++i)
        ASSERT_DOUBLE_EQ(0.5,m.facets.at(i).area)<<i;
}

TEST_F(MeshBuilderTest, can_enumerate_vertices_of_facets_in_same_order_than_edges)
{
    const Mesh m = MeshBuilder(unit_cube()).build();
    ASSERT_EQ( m.facets.size() , m.oriented_edges_per_facet.size() ); // The tables per facet have consistent size
    for(size_t facet_index=0 ; facet_index < m.facets.size() ; ++facet_index) {
        Facet f = m.facets.at(facet_index);
        ASSERT_EQ( f.vertex_index.size() , m.oriented_edges_per_facet.at(facet_index).size() ); // the facet has as many edges as vertices
        for(size_t iv=0 ; iv < f.vertex_index.size() ; ++iv ) {
            ASSERT_EQ( f.vertex_index[iv] ,
                       m.first_vertex_of_oriented_edge( m.oriented_edges_per_facet.at(facet_index).at(iv) ));
            size_t iv_next = (iv+1) % f.vertex_index.size();
            ASSERT_EQ( f.vertex_index[iv_next] ,
                       m.second_vertex_of_oriented_edge( m.oriented_edges_per_facet.at(facet_index).at(iv) ));
        }
    }
}

TEST_F(MeshBuilderTest, the_facets_of_an_edge_and_the_edges_of_a_facet)
{
    const Mesh m = MeshBuilder(unit_cube()).build();
    ASSERT_EQ(m.nb_of_static_edges , m.facets_per_edge.size()); // The tables per edge have consistent size
    for(size_t edge_index=0 ; edge_index < m.facets_per_edge.size() ; ++edge_index) {
        for(size_t fi=0 ; fi < m.facets_per_edge.at(edge_index).size() ; ++fi ) {
            size_t facet_index = m.facets_per_edge.at(edge_index).at(fi);
            auto oriented_edges_of_this_facet = m.oriented_edges_per_facet.at( facet_index );
            std::vector<size_t> edges_of_this_facet;
            std::transform(oriented_edges_of_this_facet.begin(),oriented_edges_of_this_facet.end(),std::back_inserter(edges_of_this_facet),get_oriented_edge_index);
            ASSERT_EQ( 1,  std::count( edges_of_this_facet.begin(),edges_of_this_facet.end() , edge_index )) << "edge_index = " << edge_index;
        }
    }
    for(size_t facet_index=0 ; facet_index < m.oriented_edges_per_facet.size() ; ++facet_index) {
        for(size_t ie=0 ; ie < m.oriented_edges_per_facet.at(facet_index).size() ; ++ie ) {
            size_t edge_index = Mesh::convert_oriented_edge_id_to_edge_index(m.oriented_edges_per_facet.at(facet_index).at(ie));
            auto facets_of_this_edge = m.facets_per_edge.at( edge_index );
            ASSERT_EQ( 1,  std::count( facets_of_this_edge.begin(),facets_of_this_edge.end() , facet_index )) << "facet_index = " << facet_index;
        }
    }
}

TEST_F(MeshBuilderTest, two_triangles)
{
    const Mesh m = MeshBuilder(two_triangles()).build();
    ASSERT_EQ(2, m.nb_of_static_facets);
    ASSERT_EQ(4, m.nb_of_static_nodes);
    ASSERT_EQ(5, m.nb_of_static_edges);
    // Check first point
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(0,0));
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(1,0));
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(2,0));
    // Check second point
    ASSERT_DOUBLE_EQ(4, (double)m.nodes(0,1));
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(1,1));
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(2,1));
    // Check third point
    ASSERT_DOUBLE_EQ(2, (double)m.nodes(0,2));
    ASSERT_DOUBLE_EQ(2, (double)m.nodes(1,2));
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(2,2));
    // Check fourth point
    ASSERT_DOUBLE_EQ(2, (double)m.nodes(0,3));
    ASSERT_DOUBLE_EQ(-1, (double)m.nodes(1,3));
    ASSERT_DOUBLE_EQ(0, (double)m.nodes(2,3));
    // Check first facet
    ASSERT_EQ(3,m.facets.at(0).vertex_index.size());
    ASSERT_EQ(0,m.facets.at(0).vertex_index.at(0));
    ASSERT_EQ(1,m.facets.at(0).vertex_index.at(1));
    ASSERT_EQ(2,m.facets.at(0).vertex_index.at(2));
    // Check second facet
    ASSERT_EQ(3,m.facets.at(1).vertex_index.size());
    ASSERT_EQ(0,m.facets.at(1).vertex_index.at(0));
    ASSERT_EQ(3,m.facets.at(1).vertex_index.at(1));
    ASSERT_EQ(1,m.facets.at(1).vertex_index.at(2));
}

TEST_F(MeshBuilderTest, cannot_build_mesh_if_facets_have_one_or_two_points)
{
    ASSERT_THROW(MeshBuilder(n_gone(1)).build(),InternalErrorException);
    ASSERT_THROW(MeshBuilder(n_gone(2)).build(),InternalErrorException);
}

TEST_F(MeshBuilderTest, can_build_empty_mesh)
{
    ASSERT_NO_THROW(MeshBuilder(VectorOfVectorOfPoints()).build());
    ASSERT_NO_THROW(MeshBuilder(VectorOfPoints()).build());
}

TEST_F(MeshBuilderTest, can_build_n_gone)
{
    ASSERT_NO_THROW(MeshBuilder(n_gone(1000)).build());
}

TEST_F(MeshBuilderTest, bug_when_building_trapezium)
{
    ASSERT_NO_THROW(MeshBuilder(trapezium()).build());
}

TEST_F(MeshBuilderTest, convert_stl_files_to_code)
{
    Mesh mesh = MeshBuilder(generated_stl()).build();
    ASSERT_EQ(12, mesh.facets.size());
    ASSERT_EQ(8, mesh.nodes.cols());
    ASSERT_EQ(3, mesh.nodes.rows());
}
