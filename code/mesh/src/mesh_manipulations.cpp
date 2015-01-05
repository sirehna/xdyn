/*
 * mesh_manipulations.cpp
 *
 *  Created on: Apr 29, 2014
 *      Author: cady
 */

#include <boost/foreach.hpp>

#include "mesh_manipulations.hpp"
#include "MeshException.hpp"
#include <ssc/numeric.hpp>

double area(const Matrix3x& M, //!< Matrix containing (amongst others), the points of interest
            const int idxA,    //!< Index of the column containing the first point
            const int idxB,    //!< Index of the column containing the second point
            const int idxC     //!< Index of the column containing the third point
            )
{
    const double x1 = M(0,idxB)-M(0,idxA);
    const double x2 = M(1,idxB)-M(1,idxA);
    const double x3 = M(2,idxB)-M(2,idxA);
    const double y1 = M(0,idxC)-M(0,idxA);
    const double y2 = M(1,idxC)-M(1,idxA);
    const double y3 = M(2,idxC)-M(2,idxA);
    const double A = x2*y3-x3*y2;
    const double B = x3*y1-x1*y3;
    const double C = x1*y2-x2*y1;
    return 0.5*sqrt(A*A+B*B+C*C);
}

double area(const Matrix3x& points)
{
    const int n = (int)points.cols();
    double a = 0;
    for (int i = 2 ; i < n ; ++i)
    {
        a += area(points, 0, i-1, i);
    }
    return a;
}

double area(const Matrix3x& points,std::vector<size_t> &vertex_index)
{
    const size_t n = vertex_index.size();
    double a = 0;
    for (size_t i = 2 ; i < n ; ++i)
    {
        a += area(points, (int)vertex_index[0],
                          (int)vertex_index[i-1],
                          (int)vertex_index[i]);
    }
    return a;
}

Eigen::Vector3d barycenter(const Matrix3x& p)
{
    return p.rowwise().sum().array()/double(p.cols());
}

Eigen::Vector3d barycenter(const Matrix3x& p,std::vector<size_t> &vertex_index)
{
    double x = 0;
    double y = 0;
    double z = 0;
    const size_t n = vertex_index.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        x += p(0, (int)vertex_index[i]);
        y += p(1, (int)vertex_index[i]);
        z += p(2, (int)vertex_index[i]);
    }
    return Eigen::Vector3d(x/double(n),y/double(n),z/double(n));
}

Eigen::Vector3d barycenter(const VectorOfVectorOfPoints& points //!< List of points
                          )
{
    double x = 0;
    double y = 0;
    double z = 0;
    size_t N = 0;
    const size_t n = points.size();
    for (size_t i = 0 ; i < n ; ++i)
    {
        const size_t p = points[i].size();
        for (size_t j = 0 ; j < p ; ++j)
        {
            x += points[i][j](0);
            y += points[i][j](1);
            z += points[i][j](2);
            ++N;
        }
    }
    return Eigen::Vector3d(x/double(N),y/double(N),z/double(N));
}

Eigen::Vector3d unit_normal(const Matrix3x& points)
{
    //const double sign = oriented_clockwise ? -1 : 1;
    if (points.cols() < 3)
    {
        std::stringstream ss;
        ss << "Need at least three points to define a surface: cannot compute normal vector. Input has "
           << points.cols() << " point";
        if (points.cols()>1) ss << "s";
        ss << ".";
       THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    const double x1 = points(0,1)-points(0,0);
    const double x2 = points(1,1)-points(1,0);
    const double x3 = points(2,1)-points(2,0);
    const double y1 = points(0,2)-points(0,0);
    const double y2 = points(1,2)-points(1,0);
    const double y3 = points(2,2)-points(2,0);
    const double A = x2*y3-x3*y2;
    const double B = x3*y1-x1*y3;
    const double C = x1*y2-x2*y1;

    const double norm = sqrt(A*A+B*B+C*C);
    if (norm<1000*std::numeric_limits<double>::epsilon()) return Eigen::Vector3d(0,0,0);
    return Eigen::Vector3d(A/norm,B/norm,C/norm);
}

Eigen::Vector3d unit_normal(const Matrix3x& points, //!< Polygon for which the unit normal vector is computed
                            const std::vector<size_t> &vertex_index
                           )
{
    if (vertex_index.size() < 3)
    {
        std::stringstream ss;
        ss << "Need at least three points to define a surface: cannot compute normal vector. Input has "
           << vertex_index.size() << " point";
        if (vertex_index.size()>1) ss << "s";
        ss << ".";
       THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    }
    const double x1 = points(0,(int)vertex_index[1])-points(0,(int)vertex_index[0]);
    const double x2 = points(1,(int)vertex_index[1])-points(1,(int)vertex_index[0]);
    const double x3 = points(2,(int)vertex_index[1])-points(2,(int)vertex_index[0]);
    const double y1 = points(0,(int)vertex_index[2])-points(0,(int)vertex_index[0]);
    const double y2 = points(1,(int)vertex_index[2])-points(1,(int)vertex_index[0]);
    const double y3 = points(2,(int)vertex_index[2])-points(2,(int)vertex_index[0]);
    const double A = x2*y3-x3*y2;
    const double B = x3*y1-x1*y3;
    const double C = x1*y2-x2*y1;

    const double norm = sqrt(A*A+B*B+C*C);
    if (norm<1000*std::numeric_limits<double>::epsilon()) return Eigen::Vector3d(0,0,0);
    return Eigen::Vector3d(A/norm,B/norm,C/norm);
}

Eigen::Vector3d centre_of_gravity(const Matrix3x& polygon //!< Polygon we wish to compute the centre of gravity of
                                 )
{
    const int n = (int)polygon.cols();
    Eigen::Vector3d areas_times_points(0,0,0);
    double areas = 0;
    for (int i = 2 ; i < n ; ++i)
    {
        const double S = area(polygon, 0, i-1, i);
        areas += S;
        areas_times_points += S*(polygon.col(0)+polygon.col(i-1)+polygon.col(i))/3.;
    }
    return areas_times_points/areas;
}

Matrix3x convert(const VectorOfPoints& v)
{
    Matrix3x ret(3,v.size());
    for (size_t j = 0 ; j < v.size() ; ++j)
    {
        ret.col((int)j) = v[j];
    }
    return ret;
}
#include <ssc/macros.hpp>
bool oriented_clockwise(const VectorOfVectorOfPoints& v, const EPoint& O)
{
    if (v.size() < 2) return true;
    size_t nb_of_clockwise = 0;
    size_t nb_of_anticlockwise = 0;
    bool first_facet_is_oriented_clockwise = oriented_clockwise(v.front(),O);
    if (first_facet_is_oriented_clockwise) nb_of_clockwise++;
    else nb_of_anticlockwise++;
    for (size_t i = 1 ; i < v.size() ; ++i)
    {
        const bool facet_i_is_oriented_clockwise = oriented_clockwise(v[i],O);
        if (facet_i_is_oriented_clockwise) nb_of_clockwise++;
        else nb_of_anticlockwise++;
    }
    if (nb_of_clockwise > 10*nb_of_anticlockwise) return true;
    if (nb_of_anticlockwise > 10*nb_of_clockwise) return false;
    std::stringstream ss;
    ss << "Not all facets have the same orientation: " << nb_of_clockwise << " facets seem to be oriented clockwise, but "
       << nb_of_anticlockwise << " facets seem to be oriented anticlockwise.";
    THROW(__PRETTY_FUNCTION__, MeshException, ss.str());
    return first_facet_is_oriented_clockwise;
}

bool oriented_clockwise(const VectorOfPoints& facet, //!< Points to convert
                        const EPoint& G //!< Point inside the volume (eg. its centre of gravity)
        )
{
    if (facet.size() < 3) return true;
    const Matrix3x M = convert(facet);
    const Eigen::Vector3d C = barycenter(M);
    const Eigen::Vector3d n = unit_normal(M);
    return n.dot(C-G)<=0;
}

void write_binary_stl(const VectorOfVectorOfPoints& stl, std::ostream& os)
{
    //const std::string header(80, ' ');
    //os.write(header.c_str(), 80);
    for (size_t i = 0 ; i < 80 ; ++i) os.put(1);
    const unsigned int nFaces = (unsigned int)stl.size();
    os.write(reinterpret_cast<const char*>(&nFaces), sizeof(nFaces));

    typedef unsigned short uint16;
    const uint16 spacer = 0;
    // Every Face is 50 Bytes: Normal(3*float), Vertices(9*float), 2 Bytes Spacer
    for (size_t i=0; i<nFaces; ++i)
    {
        Eigen::Matrix3d M;
        for (size_t j = 0 ; j < 3 ; ++j)
        {
            for (size_t k = 0 ; k < 3 ; ++k) M((int)k,(int)j) = stl[i][j]((int)k);
        }
        const Eigen::Vector3d normal = unit_normal(M);
        float x = (float)normal(0);
        float y = (float)normal(1);
        float z = (float)normal(2);
        os.write(reinterpret_cast<const char*>(&x), 4);
        os.write(reinterpret_cast<const char*>(&y), sizeof(float));
        os.write(reinterpret_cast<const char*>(&z), sizeof(float));
        for (int j = 0 ; j < 3 ; ++j)
        {
            float x = (float)M(0,j);
            float y = (float)M(1,j);
            float z = (float)M(2,j);
            os.write(reinterpret_cast<const char*>(&x), 4);
            os.write(reinterpret_cast<const char*>(&y), 4);
            os.write(reinterpret_cast<const char*>(&z), 4);
        }
        os.write(reinterpret_cast<const char*>(&spacer), 2);
    }
}

Eigen::Matrix3d inertia_of_triangle(
        const EPoint vertex1,  //!< first vertex of triangle expressed in inertia frame R1
        const EPoint vertex2,  //!< second vertex of triangle
        const EPoint vertex3   //!< third vertex of triangle
        )
{
    Eigen::Matrix3d JR1;
    JR1.fill(0);
    double x12 = vertex1[0] + vertex2[0];
    double x13 = vertex1[0] + vertex3[0];
    double x23 = vertex2[0] + vertex3[0];
    double y12 = vertex1[1] + vertex2[1];
    double y13 = vertex1[1] + vertex3[1];
    double y23 = vertex2[1] + vertex3[1];
    JR1(0,0) = (1.0/12.0) * ( x12*x12 + x13*x13 + x23*x23 );
    JR1(0,1) = (1.0/12.0) * ( x12*y12 + x13*y13 + x23*y23 );
    JR1(1,0) = (1.0/12.0) * ( x12*y12 + x13*y13 + x23*y23 );
    JR1(1,1) = (1.0/12.0) * ( y12*y12 + y13*y13 + y23*y23 );
    return JR1;
}

Eigen::Matrix3d inertia_of_polygon(
        const Matrix3x& verticesInR1  //!< polygon with vertices expressed in inertia frame R1
        )
{
    const int nVertices = (int)verticesInR1.cols();
    if (nVertices < 3) return Eigen::Matrix3d();

    Eigen::Matrix3d total_inertia;
    total_inertia.fill(0);
    double total_area = 0;
    for (int i = 2 ; i < nVertices ; ++i)
    {
        const double S = area(verticesInR1, 0, i-1, i);
        EPoint G = (verticesInR1.col(0)+verticesInR1.col(i-1)+verticesInR1.col(i))/3.;
        Eigen::Matrix3d JR1 = inertia_of_triangle(verticesInR1.col(0),verticesInR1.col(i-1),verticesInR1.col(i));
        total_inertia += JR1*S;
        total_area    += S;
    }
    return total_inertia/total_area;
}

double average_immersion(const std::vector<size_t>& idx,    //!< Indices of the points
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

EPoint normal_to_free_surface(
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

Eigen::Matrix3d facet_trihedron(
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

EPoint exact_application_point(
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

Matrix3x project_facet_on_free_surface(
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

Eigen::Matrix3d get_inertia_of_polygon_wrt(
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
