#include <algorithm> //std::all_of
#include <numeric> //std::accumulate

#include "ClosingFacetComputer.hpp"
#include "MeshBuilder.hpp"
#include "MeshIntersector.hpp"
#include "MeshIntersectorException.hpp"
#include "mesh_manipulations.hpp"
#include "2DMeshDisplay.hpp"
#include <ssc/macros/SerializeMapsSetsAndVectors.hpp>

Facet flip(Facet facet);
Facet flip(Facet facet)
{
    std::reverse(facet.vertex_index.begin(), facet.vertex_index.end());
    facet.unit_normal = -facet.unit_normal;
    return facet;
}

MeshIntersector::MeshIntersector(const VectorOfVectorOfPoints& mesh_, const bool check_orientation) :
 mesh(MeshPtr(new Mesh(MeshBuilder(mesh_, check_orientation).build())))
,all_relative_immersions()
,all_absolute_wave_elevations()
,all_absolute_immersions()
,index_of_emerged_facets()
,index_of_immersed_facets()
,index_of_facets_exactly_on_the_surface()
,index_of_edges_exactly_on_surface()
{}

MeshIntersector::MeshIntersector(const MeshPtr mesh_)
        :mesh(mesh_)
        ,all_relative_immersions()
        ,all_absolute_wave_elevations()
        ,all_absolute_immersions()
        ,index_of_emerged_facets()
        ,index_of_immersed_facets()
        ,index_of_facets_exactly_on_the_surface()
        ,index_of_edges_exactly_on_surface()
{}

void MeshIntersector::find_intersection_with_free_surface(
        std::vector<size_t>& split_edges,
        std::vector<int>& edges_immersion_status,
        std::vector<bool>& facet_crosses_free_surface)
{
    for (size_t edge_index = 0; edge_index < mesh->nb_of_static_edges; ++edge_index)
    {
        double z0 = all_relative_immersions[mesh->edges[0][edge_index]];
        double z1 = all_relative_immersions[mesh->edges[1][edge_index]];
        int status = get_edge_immersion_status(z0, z1);
        edges_immersion_status[edge_index] = status;
        if (crosses_free_surface(status))
        {
            split_edges[edge_index] = split_partially_immersed_edge(edge_index, edges_immersion_status);
        }
        if (   crosses_free_surface(status)
            or both_ends_just_touch_free_surface(status)
            or one_of_the_ends_just_touches_free_surface(status))
        {
            for (auto facet_index:mesh->facets_per_edge[edge_index])
            {
                facet_crosses_free_surface[facet_index] = true;
            }
        }
    }
}

void MeshIntersector::classify_or_split(
        const std::vector<size_t>& split_edges,
        const std::vector<bool>& facet_crosses_free_surface,
        std::vector<int>& edges_immersion_status)
{
    // Iterate on each facet to classify and/or split
    for (size_t facet_index = 0 ; facet_index < mesh->nb_of_static_facets ; ++facet_index)
    {
        if (facet_crosses_free_surface[facet_index])
        {
            split_partially_immersed_facet_and_classify(facet_index,
                                                        edges_immersion_status,
                                                        split_edges);
        }
        else
        {
            classify_facet(facet_index, edges_immersion_status);
        }
    }
}

void MeshIntersector::classify_facet(const size_t facet_index, const std::vector<int>& edges_immersion_status)
{
    // Each edge contains exactly two nodes: as the edge does not cross the free
    // surface in this branch of the 'if' statement, the immersion status of both
    // nodes is the same and is equal to the immersion status of the corresponding
    // edge.
    const size_t index_of_first_node_in_edge = mesh->oriented_edges_per_facet[facet_index][0];
    const size_t index_of_edge_containing_first_node_in_edge = Mesh::convert_oriented_edge_id_to_edge_index(index_of_first_node_in_edge);
    const int immersion_status_of_first_node_in_edge = edges_immersion_status[index_of_edge_containing_first_node_in_edge];
    if (is_emerged(immersion_status_of_first_node_in_edge)) index_of_emerged_facets.push_back(facet_index);
    else                                                    index_of_immersed_facets.push_back(facet_index);
}

void MeshIntersector::reset_dynamic_members()
{
    mesh->reset_dynamic_data();
    index_of_emerged_facets.clear();
    index_of_immersed_facets.clear();
    index_of_facets_exactly_on_the_surface.clear();
    index_of_emerged_facets.reserve(mesh->facets.size());
    index_of_immersed_facets.reserve(mesh->facets.size());
    index_of_facets_exactly_on_the_surface.reserve(mesh->facets.size());
    index_of_edges_exactly_on_surface.clear();
}

void MeshIntersector::update_intersection_with_free_surface(const std::vector<double>& relative_immersions,
        const std::vector<double>& absolute_wave_elevations  //!< z coordinate in NED frame of the free surface for each point in mesh
        )
{
    all_relative_immersions = relative_immersions;
    if (std::any_of(relative_immersions.begin(),relative_immersions.end(), [](const double x){return std::isnan(x);}))
    {
        THROW(__PRETTY_FUNCTION__, MeshIntersectorException, "Some relative immersions are NaN.");
    }
    all_absolute_wave_elevations = absolute_wave_elevations;
    reset_dynamic_members();
    std::vector<bool> facet_crosses_free_surface(mesh->nb_of_static_facets,false);
    std::vector<int> edges_immersion_status(mesh->nb_of_static_edges,0); // the immersion status of each edge
    std::vector<size_t > split_edges(mesh->nb_of_static_edges,0);  //!< a table indicating the index of replacing edge for each edge that is split (there are two consecutive edges per split edge, the table only gives the first one)
    find_intersection_with_free_surface(split_edges, edges_immersion_status, facet_crosses_free_surface);
    classify_or_split(split_edges, facet_crosses_free_surface, edges_immersion_status);
    all_absolute_immersions.resize(all_absolute_wave_elevations.size());
    for (size_t i = 0 ; i < all_absolute_wave_elevations.size() ; ++i)
    {
        all_absolute_immersions[i] = all_relative_immersions[i] + all_absolute_wave_elevations[i];
    }
    build_closing_edge();
}

void MeshIntersector::build_closing_edge()
{
    ClosingFacetComputer::ListOfEdges all_edges_as_pairs;
    for (size_t idx = 0 ; idx < mesh->edges.at(0).size() ; ++idx)
    {
        all_edges_as_pairs.push_back(std::make_pair(mesh->edges.at(0).at(idx), mesh->edges.at(1).at(idx)));
    }
    if (index_of_edges_exactly_on_surface.empty()) return;
    const auto ll = ClosingFacetComputer::group_connected_edges(all_edges_as_pairs, std::vector<size_t>(index_of_edges_exactly_on_surface.begin(),index_of_edges_exactly_on_surface.end()));
    for (const auto l:ll)
    {
        const ClosingFacetComputer c(&mesh->all_nodes, all_edges_as_pairs, l);
        const auto contour = c.contour();
        if (not(contour.edge_idx.empty()))
        {
            EPoint unit_normal(0,0,-1);
            std::vector<size_t> contour_with_oriented_edge_ids;
            for (size_t i = 0 ; i < contour.edge_idx.size() ; ++i)
            {
                const size_t idx = contour.edge_idx.at(i);
                const bool reversed = contour.reversed.at(i);
                contour_with_oriented_edge_ids.push_back(Mesh::convert_index_to_oriented_edge_id(idx,reversed));
            }
            index_of_facets_exactly_on_the_surface.push_back(mesh->create_facet_from_edges(contour_with_oriented_edge_ids,unit_normal));
        }
    }
}

void MeshIntersector::split_partially_immersed_facet_and_classify(
        size_t facet_index,                             //!< index of facet to split
        const std::vector<int>& edges_immersion_status, //!< immersion status of each edge
        const std::vector<size_t>& split_edges          //!< replacement map for split edges
        )
{
    const std::vector<size_t> oriented_edges_of_this_facet = mesh->oriented_edges_per_facet[facet_index];
    std::vector<size_t> emerged_edges;
    std::vector<size_t> immersed_edges;
    int status=-1;
    size_t first_emerged  = 0;
    size_t first_immersed = 0;

    for(auto oriented_edge:oriented_edges_of_this_facet)
    {
        size_t edge_index=Mesh::convert_oriented_edge_id_to_edge_index(oriented_edge);
        // Handle degenerate case
        if (both_ends_just_touch_free_surface(edges_immersion_status[edge_index]))
        {
            emerged_edges.push_back(oriented_edge);
            immersed_edges.push_back(oriented_edge);
            index_of_edges_exactly_on_surface.insert(edge_index);
            if(status==3) first_emerged = emerged_edges.size();
            if(status==0) first_immersed = immersed_edges.size();
        }
        else if (is_emerged(edges_immersion_status[edge_index]))
        {
            if(status==3) first_emerged = emerged_edges.size();
            emerged_edges.push_back(oriented_edge);
            status = 0;
        }
        else if (is_immersed(edges_immersion_status[edge_index]))
        {
            if(status==0) first_immersed = immersed_edges.size();
            immersed_edges.push_back(oriented_edge);
            status = 3;
        }
        else
        {
            bool reverse_direction=Mesh::get_oriented_edge_direction(oriented_edge);
            size_t edge1 = split_edges[edge_index];
            size_t edge2 = edge1 + 1; // because edges are always added by pair...
            if(reverse_direction) std::swap(edge1,edge2);
            if(is_emerged(edges_immersion_status[edge1]))
            {
                emerged_edges.push_back(Mesh::convert_index_to_oriented_edge_id(edge1,reverse_direction));
                first_immersed = immersed_edges.size();
                immersed_edges.push_back(Mesh::convert_index_to_oriented_edge_id(edge2,reverse_direction));
                status = 3;
            }
            else
            {
                immersed_edges.push_back(Mesh::convert_index_to_oriented_edge_id(edge1,reverse_direction));
                first_emerged = emerged_edges.size();
                emerged_edges.push_back(Mesh::convert_index_to_oriented_edge_id(edge2,reverse_direction));
                status = 0;
            }
        }
    }
    // handle the degenerated cases, when the facet is tangent to free surface
    if(emerged_edges.size() <= 1)
    {
        index_of_immersed_facets.push_back(facet_index);
        return;
    }
    if(immersed_edges.size() <= 1)
    {
        index_of_emerged_facets.push_back(facet_index);
        return;
    }
    // insert the closing edge
    size_t closing_edge_index = mesh->add_edge(
            mesh->first_vertex_of_oriented_edge(immersed_edges[first_immersed]),
            mesh->first_vertex_of_oriented_edge( emerged_edges[ first_emerged]));
    const bool closing_edge_is_a_point = mesh->edges[0][closing_edge_index] == mesh->edges[1][closing_edge_index];
    if (not(closing_edge_is_a_point))
        index_of_edges_exactly_on_surface.insert(closing_edge_index);
    immersed_edges.insert(immersed_edges.begin() + (long)first_immersed, Mesh::convert_index_to_oriented_edge_id(closing_edge_index,true));
    emerged_edges.insert( emerged_edges.begin()  + (long)first_emerged,  Mesh::convert_index_to_oriented_edge_id(closing_edge_index,false));

    // create the Facets
    EPoint unit_normal=(mesh->facets.begin()+(long)facet_index)->unit_normal;
    index_of_emerged_facets.push_back(mesh->create_facet_from_edges(emerged_edges,unit_normal));
    index_of_immersed_facets.push_back(mesh->create_facet_from_edges(immersed_edges,unit_normal));
}

size_t MeshIntersector::split_partially_immersed_edge(
        const size_t edge_index ,                //!< the index of edge to be split
        std::vector<int> &edges_immersion_status //!< the immersion status of each edge
        )
{
    size_t first_vertex_index = mesh->edges[0][edge_index];
    size_t last_vertex_index  = mesh->edges[1][edge_index];
    EPoint A=mesh->all_nodes.col((int)first_vertex_index);
    EPoint B=mesh->all_nodes.col((int)last_vertex_index);
    const double dzA=all_relative_immersions[first_vertex_index];
    const double dzB=all_relative_immersions[last_vertex_index];
    const double zA = all_absolute_wave_elevations[first_vertex_index];
    const double zB = all_absolute_wave_elevations[last_vertex_index];
    size_t mid_vertex_index = mesh->add_vertex(edge_intersection(A,dzA,B,dzB));
    all_relative_immersions.push_back(0);
    const double k = dzA/(dzA-dzB);
    all_absolute_wave_elevations.push_back(zA+k*(zB-zA));
    size_t first_sub_edge_index = mesh->add_edge(first_vertex_index,mid_vertex_index);
    /* second_sub_edge_index = */ mesh->add_edge(mid_vertex_index,last_vertex_index);
    edges_immersion_status.push_back(((edges_immersion_status[edge_index] & 1)  *3) | 4);
    edges_immersion_status.push_back(((edges_immersion_status[edge_index] & 2)/2*3) | 4);
    return first_sub_edge_index;
}

Matrix3x MeshIntersector::coordinates_of_facet(size_t facet_index) const
{
    std::vector<Facet>::const_iterator facet=mesh->facets.begin()+(long)facet_index;
    size_t n = facet->vertex_index.size();
    Matrix3x coord(3,n);
    for(size_t i=0;i<n;++i)
        coord.col((int)i) = mesh->all_nodes.col((int)facet->vertex_index[i]);
    return coord;
}

std::vector<double> MeshIntersector::immersions_of_facet(size_t facet_index) const
{
    std::vector<Facet>::const_iterator facet=mesh->facets.begin()+(int)facet_index;
    size_t n = facet->vertex_index.size();
    std::vector<double> z(n,0.0);
    for(size_t i=0;i<n;++i)
        z[i] = all_relative_immersions[facet->vertex_index[i]];
    return z;
}

EPoint MeshIntersector::edge_intersection(const EPoint& A, const double dzA, const EPoint& B, const double dzB)
{
    const double xA = A(0);
    const double xB = B(0);
    const double yA = A(1);
    const double yB = B(1);
    const double zA = A(2);
    const double zB = B(2);
    const double k = dzA/(dzA-dzB);
    return EPoint(xA + k*(xB-xA),
                  yA + k*(yB-yA),
                  zA + k*(zB-zA)
                  );
}

int MeshIntersector::get_edge_immersion_status(
        const double z0, //!< Relative immersion of first vertex (in m)
        const double z1  //!< Relative immersion of second vertex (in m)
        )
{
    bool first_is_immersed  = z0 > 0;
    bool second_is_immersed = z1 > 0;
    bool first_is_emerged = z0 < 0;
    bool second_is_emerged = z1 < 0;
    return (first_is_immersed?1:0) | (second_is_immersed?2:0) | (first_is_emerged?4:0) | (second_is_emerged?8:0);
}

bool MeshIntersector::crosses_free_surface(int status)
{
    return (status==9) or (status==6);
}

bool MeshIntersector::is_emerged(int status)
{
    return (status==12) or (status==8) or (status==4);
}

bool MeshIntersector::is_immersed(int status)
{
    return (status==1) or (status==2) or (status==3);
}

bool MeshIntersector::one_of_the_ends_just_touches_free_surface(int status)
{
    return (status==1) or (status==2) or (status==4) or (status==8);
}

bool MeshIntersector::both_ends_just_touch_free_surface(int status)
{
    return status==0;
}

bool MeshIntersector::has(const Facet& f, //!< Facet to check
                          const FacetIterator& begin,
                          const FacetIterator& end
                         ) const
{
    if (f.vertex_index.empty()) return false;

    const auto facet_contains_vertex = [](const size_t vertex_to_test, const Facet& facet, const Eigen::Vector3d& unit_normal) -> bool
                                     {
                                         if ((unit_normal-facet.unit_normal).norm()>1E-8) return false;
                                         for (const auto current_vertex:facet.vertex_index)
                                         {
                                             if (current_vertex == vertex_to_test) return true;
                                         }
                                         return false;
                                     };

    const auto at_least_one_facet_contains_vertex = [&facet_contains_vertex](const size_t vertex_to_test, const FacetIterator& begin, const FacetIterator& end, const Eigen::Vector3d& unit_normal) -> bool
                                                  {
                                                      for (auto that_facet = begin ; that_facet != end ; ++that_facet)
                                                      {
                                                          if (facet_contains_vertex(vertex_to_test, *that_facet, unit_normal)) return true;
                                                      }
                                                      return false;
                                                  };

    for (const auto vertex:f.vertex_index)
    {
        if (not(at_least_one_facet_contains_vertex(vertex, begin, end, f.unit_normal))) return false;
    }
    return true;
}

bool MeshIntersector::has(const Facet& f //!< Facet to check
                         ) const
{
    if (f.vertex_index.empty())                   return false;
    if (has(f, begin_immersed(), end_immersed())) return true;
    if (has(f, begin_emerged(), end_emerged()))   return true;
                                                  return false;
}
using namespace ssc::kinematics;
RotationMatrix rot_(const double phi, const double theta, const double psi);
RotationMatrix rot_(const double phi, const double theta, const double psi)
{
    return rotation_matrix<INTRINSIC, CHANGING_ANGLE_ORDER, 3, 2, 1>(ssc::kinematics::EulerAngles(phi,theta,psi));
}

CenterOfMass MeshIntersector::center_of_mass_immersed() const
{
    return center_of_mass(begin_immersed(), end_immersed(), true);
}

CenterOfMass MeshIntersector::center_of_mass_emerged() const
{
    return center_of_mass(begin_emerged(), end_emerged(), false);
}

CenterOfMass MeshIntersector::center_of_mass(const FacetIterator& begin, const FacetIterator& end, const bool immersed) const
{
    CenterOfMass ret(EPoint(0,0,0), 0);
    if (begin==end) return ret;
    auto ref_normal_vector = begin->unit_normal;
    ret.all_facets_are_in_same_plane = true;
    for (auto that_facet = begin ; that_facet != end ; ++that_facet)
    {
        ret += center_of_mass(*that_facet);
        const bool current_facet_has_same_normal_as_ref = ref_normal_vector.dot(that_facet->unit_normal) > 1-1E-6;
        ret.all_facets_are_in_same_plane = ret.all_facets_are_in_same_plane and current_facet_has_same_normal_as_ref;
    }
    for (auto that_facet = begin_surface() ; that_facet != end_surface() ; ++that_facet)
    {
        const auto closing_facet = immersed ? *that_facet : flip(*that_facet);
        if (not(has(closing_facet, begin, end)))
        {
            ret += center_of_mass(closing_facet);
        }
    }
    if (ret.volume!=0) ret.G /= ret.volume;
    ret.volume = std::abs(ret.volume);
    return ret;
}

Eigen::MatrixXd MeshIntersector::convert(const Facet& f) const
{
    Eigen::MatrixXd ret(3, f.vertex_index.size());
    for (size_t j = 0 ; j < f.vertex_index.size() ; ++j)
    {
        ret.col((int)j) = mesh->all_nodes.col((int)f.vertex_index[j]);
    }
    return ret;
}

Facet MeshIntersector::make(const Facet& f, const size_t i1, const size_t i2, const size_t i3) const
{
    Facet f_;
    f_.vertex_index.push_back(i1);
    f_.vertex_index.push_back(i2);
    f_.vertex_index.push_back(i3);
    f_.unit_normal = f.unit_normal;
    const auto M = convert(f_);
    f_.area = area(M);
    f_.centre_of_gravity = centre_of_gravity(M);
    return f_;
}

CenterOfMass MeshIntersector::center_of_mass(const Facet& f) const
{
    double totalVolume = 0, currentVolume;
    double xCenter = 0, yCenter = 0, zCenter = 0;

    const EPoint P1 = mesh->all_nodes.col((int)f.vertex_index.at(0));
    for (size_t i = 2; i < f.vertex_index.size() ; i++)
    {
        const EPoint P2 = mesh->all_nodes.col((int)f.vertex_index.at(i-1));
        const EPoint P3 = mesh->all_nodes.col((int)f.vertex_index.at(i));
        totalVolume += currentVolume = (P1(0)*P2(1)*P3(2) - P1(0)*P3(1)*P2(2) - P2(0)*P1(1)*P3(2) + P2(0)*P3(1)*P1(2) + P3(0)*P1(1)*P2(2) - P3(0)*P2(1)*P1(2)) / 6;
        xCenter += ((P1(0) + P2(0) + P3(0)) / 4) * currentVolume;
        yCenter += ((P1(1) + P2(1) + P3(1)) / 4) * currentVolume;
        zCenter += ((P1(2) + P2(2) + P3(2)) / 4) * currentVolume;
    }
    if (totalVolume!=0) return CenterOfMass(EPoint(xCenter/totalVolume,yCenter/totalVolume,zCenter/totalVolume), totalVolume);
                        return CenterOfMass(EPoint(0,0,0), 0);
}

double MeshIntersector::facet_volume(const Facet& f) const
{
    if (f.vertex_index.empty()) return 0;
    const auto P = mesh->all_nodes.col((int)f.vertex_index.front());
    // Dot product to get distance from point to plane
    const double height = f.unit_normal.dot(P);
    return (f.area * height) / 3.0;
}

double MeshIntersector::volume(const FacetIterator& begin, const FacetIterator& end) const
{
    double volume = 0;
    size_t n = 0;
    for (auto that_facet = begin ; that_facet != end ; ++that_facet)
    {
        volume += facet_volume(*that_facet);
        ++n;
    }
    if (n < 3) return 0;
    return volume;
}

double MeshIntersector::immersed_volume() const
{
    double V = volume(begin_immersed(), end_immersed());
    if (V == 0) return 0; // Because it means that the closing facet is equal to the immersed facets
    for (auto closing_facet=begin_surface();closing_facet!=end_surface();++closing_facet)
    {
        if (not(has(*closing_facet, begin_immersed(), end_immersed())))
        {
            const double closing_facet_volume = facet_volume(*closing_facet);
            V += closing_facet_volume;
        }
    }
    return fabs(V);
}

double MeshIntersector::emerged_volume() const
{
    double V = volume(begin_emerged(), end_emerged());
    if (V == 0) return 0; // Because it means that the closing facet is equal to the emerged facets
    for (auto closing_facet=begin_surface();closing_facet!=end_surface();++closing_facet)
    {
        const auto facet = flip(*closing_facet);
        if (not(has(facet, begin_emerged(), end_emerged())))
        {
            const double closing_facet_volume = facet_volume(facet);
            V += closing_facet_volume;
        }
    }
    return fabs(V);
}

std::string MeshIntersector::display_facet_in_NED(const Facet& facet, const EPoint& mesh_center_in_NED_frame, const ssc::kinematics::RotationMatrix& R_from_ned_to_mesh) const
{
    std::stringstream ss;
    ss << "Facet:" << std::endl
       << "Unit normal: " << facet.unit_normal.transpose() << std::endl
       << "Area: " << facet.area << std::endl
       << "Center of gravity: " << (mesh_center_in_NED_frame + R_from_ned_to_mesh*facet.centre_of_gravity).transpose() << std::endl;
    auto M = convert(facet);
    M = R_from_ned_to_mesh*M;
    for (int i = 0 ; i < M.cols(); ++i)
    {
        M.col(i) += mesh_center_in_NED_frame;
    }
    ss << "Vertex indices: " << facet.vertex_index << std::endl
       << "Coordinates in NED frame (one column per point):" << std::endl
       << M;
    return ss.str();
}

std::string MeshIntersector::display_edge_in_NED(const size_t edge_idx, const EPoint& mesh_center_in_NED_frame, const ssc::kinematics::RotationMatrix& R_from_ned_to_mesh) const
{
    std::stringstream ss;
    ss << "Edge:" << std::endl;
    const auto P1 = R_from_ned_to_mesh*(mesh->all_nodes.col(mesh->edges[0][edge_idx])) + mesh_center_in_NED_frame;
    const auto P2 = R_from_ned_to_mesh*(mesh->all_nodes.col(mesh->edges[1][edge_idx])) + mesh_center_in_NED_frame;
    Eigen::MatrixXd edge(3,2);

    edge << P1, P2;
    ss << edge << std::endl;
    return ss.str();
}
