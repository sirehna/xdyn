#include <cmath>

#include "TriMeshTestData.hpp"

VectorOfPoints one_triangle()
{
    VectorOfPoints P;
    EPoint p1, p2, p3;
    p1(0) = 1.0;
    p1(1) = 2.1;
    p1(2) = 3.2;
    p2(0) = 2.1;
    p2(1) = 3.7;
    p2(2) = 4.5;
    p3(0) = 3.1;
    p3(1) = 4.5;
    p3(2) = 6.7;
    P.push_back(p1);
    P.push_back(p2);
    P.push_back(p3);
    return P;
}

VectorOfPoints one_triangle_clockwise()
{
    VectorOfPoints P;
    EPoint p1, p2, p3;
    p1(0) = 1.0;
    p1(1) = 2.1;
    p1(2) = 3.2;
    p2(0) = 2.1;
    p2(1) = 3.7;
    p2(2) = 4.5;
    p3(0) = 3.1;
    p3(1) = 4.5;
    p3(2) = 6.7;
    P.push_back(p1);
    P.push_back(p2);
    P.push_back(p3);
    return P;
}


VectorOfPoints degenerated_triangle()
{
    VectorOfPoints P;
    EPoint p1, p2, p3;
    p1(0) = 1.0;
    p1(1) = 2.1;
    p1(2) = 3.2;
    p2(0) = 2.1;
    p2(1) = 3.7;
    p2(2) = 4.5;
    p3(0) = 2.1;
    p3(1) = 3.7;
    p3(2) = 4.5;
    P.push_back(p1);
    P.push_back(p2);
    P.push_back(p3);
    return P;
}

VectorOfVectorOfPoints unit_cube()
{
    VectorOfVectorOfPoints V;
    VectorOfPoints P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12;
    EPoint p1, p2, p3;
    p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5; // [1]
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5; // [2]
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5; // [3]
    P1.push_back(p1);
    P1.push_back(p2);
    P1.push_back(p3);
    V.push_back(P1);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5; // [1]
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5; // [3]
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5; // [4]
    P2.push_back(p1);
    P2.push_back(p2);
    P2.push_back(p3);
    V.push_back(P2);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5; // [5]
    p2(0)=-0.5; p2(1)=-0.5; p2(2)=-0.5; // [6]
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5; // [7]
    P3.push_back(p1);
    P3.push_back(p2);
    P3.push_back(p3);
    V.push_back(P3);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5; // [5]
    p2(0)=-0.5; p2(1)=+0.5; p2(2)=-0.5; // [7]
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5; // [8]
    P4.push_back(p1);
    P4.push_back(p2);
    P4.push_back(p3);
    V.push_back(P4);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5; // [6]
    p2(0)=-0.5; p2(1)=-0.5; p2(2)=+0.5; // [1]
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5; // [4]
    P5.push_back(p1);
    P5.push_back(p2);
    P5.push_back(p3);
    V.push_back(P5);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5; // [6]
    p2(0)=-0.5; p2(1)=+0.5; p2(2)=+0.5; // [4]
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5; // [7]
    P6.push_back(p1);
    P6.push_back(p2);
    P6.push_back(p3);
    V.push_back(P6);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5; // [2]
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5; // [5]
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5; // [8]
    P7.push_back(p1);
    P7.push_back(p2);
    P7.push_back(p3);
    V.push_back(P7);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5; // [2]
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5; // [8]
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5; // [3]
    P8.push_back(p1);
    P8.push_back(p2);
    P8.push_back(p3);
    V.push_back(P8);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5; // [6]
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5; // [5]
    p3(0)=+0.5; p3(1)=-0.5; p3(2)=+0.5; // [2]
    P9.push_back(p1);
    P9.push_back(p2);
    P9.push_back(p3);
    V.push_back(P9);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5; // [6]
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5; // [2]
    p3(0)=-0.5; p3(1)=-0.5; p3(2)=+0.5; // [1]
    P10.push_back(p1);
    P10.push_back(p2);
    P10.push_back(p3);
    V.push_back(P10);

    p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5; // [4]
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5; // [3]
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5; // [8]
    P11.push_back(p1);
    P11.push_back(p2);
    P11.push_back(p3);
    V.push_back(P11);

    p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5; // [4]
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5; // [8]
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5; // [7]
    P12.push_back(p1);
    P12.push_back(p2);
    P12.push_back(p3);
    V.push_back(P12);
    return V;
}

VectorOfVectorOfPoints unit_cube_clockwise()
{
    VectorOfVectorOfPoints V;
    VectorOfPoints P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12;
    EPoint p1, p2, p3;
    p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5;
    P1.push_back(p1);
    P1.push_back(p3);
    P1.push_back(p2);
    V.push_back(P1);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5;
    P2.push_back(p1);
    P2.push_back(p3);
    P2.push_back(p2);
    V.push_back(P2);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=-0.5; p2(2)=-0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
    P3.push_back(p1);
    P3.push_back(p3);
    P3.push_back(p2);
    V.push_back(P3);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=+0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
    P4.push_back(p1);
    P4.push_back(p3);
    P4.push_back(p2);
    V.push_back(P4);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=-0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5;
    P5.push_back(p1);
    P5.push_back(p3);
    P5.push_back(p2);
    V.push_back(P5);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=+0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
    P6.push_back(p1);
    P6.push_back(p3);
    P6.push_back(p2);
    V.push_back(P6);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
    P7.push_back(p1);
    P7.push_back(p3);
    P7.push_back(p2);
    V.push_back(P7);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5;
    P8.push_back(p1);
    P8.push_back(p3);
    P8.push_back(p2);
    V.push_back(P8);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=-0.5; p3(2)=+0.5;
    P9.push_back(p1);
    P9.push_back(p3);
    P9.push_back(p2);
    V.push_back(P9);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=-0.5; p3(2)=+0.5;
    P10.push_back(p1);
    P10.push_back(p3);
    P10.push_back(p2);
    V.push_back(P10);

    p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
    P11.push_back(p1);
    P11.push_back(p3);
    P11.push_back(p2);
    V.push_back(P11);

    p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
    P12.push_back(p1);
    P12.push_back(p3);
    P12.push_back(p2);
    V.push_back(P12);
    return V;
}

VectorOfVectorOfPoints unit_cube_with_incorrect_orientation()
{
    VectorOfVectorOfPoints V;
    VectorOfPoints P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12;
    EPoint p1, p2, p3;
    p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5;
    P1.push_back(p1);
    P1.push_back(p3);
    P1.push_back(p2);
    V.push_back(P1);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5;
    P2.push_back(p1);
    P2.push_back(p2);
    P2.push_back(p3);
    V.push_back(P2);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=-0.5; p2(2)=-0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
    P3.push_back(p1);
    P3.push_back(p2);
    P3.push_back(p3);
    V.push_back(P3);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=+0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
    P4.push_back(p1);
    P4.push_back(p2);
    P4.push_back(p3);
    V.push_back(P4);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=-0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=+0.5;
    P5.push_back(p1);
    P5.push_back(p3);
    P5.push_back(p2);
    V.push_back(P5);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=-0.5; p2(1)=+0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
    P6.push_back(p1);
    P6.push_back(p3);
    P6.push_back(p2);
    V.push_back(P6);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
    P7.push_back(p1);
    P7.push_back(p3);
    P7.push_back(p2);
    V.push_back(P7);

    p1(0)=+0.5; p1(1)=-0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=+0.5;
    P8.push_back(p1);
    P8.push_back(p3);
    P8.push_back(p2);
    V.push_back(P8);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=-0.5;
    p3(0)=+0.5; p3(1)=-0.5; p3(2)=+0.5;
    P9.push_back(p1);
    P9.push_back(p3);
    P9.push_back(p2);
    V.push_back(P9);

    p1(0)=-0.5; p1(1)=-0.5; p1(2)=-0.5;
    p2(0)=+0.5; p2(1)=-0.5; p2(2)=+0.5;
    p3(0)=-0.5; p3(1)=-0.5; p3(2)=+0.5;
    P10.push_back(p1);
    P10.push_back(p3);
    P10.push_back(p2);
    V.push_back(P10);

    p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=+0.5;
    p3(0)=+0.5; p3(1)=+0.5; p3(2)=-0.5;
    P11.push_back(p1);
    P11.push_back(p3);
    P11.push_back(p2);
    V.push_back(P11);

    p1(0)=-0.5; p1(1)=+0.5; p1(2)=+0.5;
    p2(0)=+0.5; p2(1)=+0.5; p2(2)=-0.5;
    p3(0)=-0.5; p3(1)=+0.5; p3(2)=-0.5;
    P12.push_back(p1);
    P12.push_back(p3);
    P12.push_back(p2);
    V.push_back(P12);
    return V;
}

VectorOfVectorOfPoints two_triangles()
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet_1, facet_2;
    facet_1.push_back(EPoint(0,0,0));
    facet_1.push_back(EPoint(4,0,0));
    facet_1.push_back(EPoint(2,2,0));

    facet_2.push_back(EPoint(0,0,0));
    facet_2.push_back(EPoint(2,-1,0));
    facet_2.push_back(EPoint(4,0,0));

    mesh.push_back(facet_1);
    mesh.push_back(facet_2);

    return mesh;
}

VectorOfVectorOfPoints two_triangles_immerged()
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet_1, facet_2;
    facet_1.push_back(EPoint(-2,+4,+6));
    facet_1.push_back(EPoint(-2,-4,+6));
    facet_1.push_back(EPoint(+2,+4,+4));

    facet_2.push_back(EPoint(+2,+4,+4));
    facet_2.push_back(EPoint(-2,-4,+6));
    facet_2.push_back(EPoint(+2,-4,+4));

    mesh.push_back(facet_1);
    mesh.push_back(facet_2);

    return mesh;
}

VectorOfVectorOfPoints trapezium()
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet;
    facet.push_back(EPoint(0,0,0));
    facet.push_back(EPoint(3,0,0));
    facet.push_back(EPoint(2,1,0));
    facet.push_back(EPoint(1,1,0));
    mesh.push_back(facet);
    return mesh;
}

VectorOfVectorOfPoints n_gone(const size_t n)
{
    const double two_pi = 8.*atan(1.);
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet;
    for (size_t i = 0 ; i < n ; ++i)
    {
        facet.push_back(EPoint(cos(double(i)*two_pi/double(n)),
                               sin(double(i)*two_pi/double(n)),0));
    }
    if (n) mesh.push_back(facet);
    return mesh;
}

VectorOfVectorOfPoints cube(const double a, const double x0, const double y0, const double z0, const bool close)
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet_1, facet_2, facet_3, facet_4, facet_5;

    const EPoint P1(x0-a/2,y0+a/2,z0-a/2);
    const EPoint P2(x0+a/2,y0+a/2,z0-a/2);
    const EPoint P3(x0+a/2,y0-a/2,z0-a/2);
    const EPoint P4(x0-a/2,y0-a/2,z0-a/2);

    const EPoint P5(x0-a/2,y0+a/2,z0+a/2);
    const EPoint P6(x0+a/2,y0+a/2,z0+a/2);
    const EPoint P7(x0+a/2,y0-a/2,z0+a/2);
    const EPoint P8(x0-a/2,y0-a/2,z0+a/2);

    facet_1.push_back(P1);
    facet_1.push_back(P2);
    facet_1.push_back(P3);
    facet_1.push_back(P4);
    mesh.push_back(facet_1);

    facet_2.push_back(P1);
    facet_2.push_back(P5);
    facet_2.push_back(P6);
    facet_2.push_back(P2);
    mesh.push_back(facet_2);

    facet_3.push_back(P2);
    facet_3.push_back(P6);
    facet_3.push_back(P7);
    facet_3.push_back(P3);
    mesh.push_back(facet_3);

    facet_4.push_back(P3);
    facet_4.push_back(P7);
    facet_4.push_back(P8);
    facet_4.push_back(P4);
    mesh.push_back(facet_4);

    facet_5.push_back(P4);
    facet_5.push_back(P8);
    facet_5.push_back(P5);
    facet_5.push_back(P1);
    mesh.push_back(facet_5);

    if (close)
    {
        VectorOfPoints facet_6;
        facet_6.push_back(P5);
        facet_6.push_back(P8);
        facet_6.push_back(P7);
        facet_6.push_back(P6);
        mesh.push_back(facet_6);
    }

    return mesh;
}

VectorOfVectorOfPoints tetrahedron(const double a, const double x, const double y, const double z)
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet_1, facet_2, facet_3, facet_4;
    const EPoint P1(x, y, z);
    const EPoint P2(-sqrt(3)*a/6 + x, -a/2 + y, sqrt(6)*a/3 + z);
    const EPoint P3(-sqrt(3)*a/6 + x, a/2 + y,  sqrt(6)*a/3 + z);
    const EPoint P4(sqrt(3)*a/3 + x , y,        sqrt(6)*a/3 + z);

    facet_1.push_back(P1);
    facet_1.push_back(P2);
    facet_1.push_back(P3);
    mesh.push_back(facet_1);

    facet_2.push_back(P1);
    facet_2.push_back(P3);
    facet_2.push_back(P4);
    mesh.push_back(facet_2);

    facet_3.push_back(P1);
    facet_3.push_back(P4);
    facet_3.push_back(P2);
    mesh.push_back(facet_3);

    facet_4.push_back(P3);
    facet_4.push_back(P2);
    facet_4.push_back(P4);
    mesh.push_back(facet_4);

    return mesh;
}

VectorOfVectorOfPoints tetrahedron_clockwise(const double a, const double x, const double y, const double z)
{
    VectorOfVectorOfPoints mesh;
    VectorOfPoints facet_1, facet_2, facet_3, facet_4;
    const EPoint P1(x, y, z);
    const EPoint P2(-sqrt(3)*a/6 + x, -a/2 + y, sqrt(6)*a/3 + z);
    const EPoint P3(-sqrt(3)*a/6 + x, a/2 + y,  sqrt(6)*a/3 + z);
    const EPoint P4(sqrt(3)*a/3 + x , y,        sqrt(6)*a/3 + z);

    facet_1.push_back(P1);
    facet_1.push_back(P3);
    facet_1.push_back(P2);
    mesh.push_back(facet_1);

    facet_2.push_back(P1);
    facet_2.push_back(P4);
    facet_2.push_back(P3);
    mesh.push_back(facet_2);

    facet_3.push_back(P1);
    facet_3.push_back(P2);
    facet_3.push_back(P4);
    mesh.push_back(facet_3);

    facet_4.push_back(P3);
    facet_4.push_back(P4);
    facet_4.push_back(P2);
    mesh.push_back(facet_4);

    return mesh;
}

VectorOfVectorOfPoints generated_stl() // Generated by convert_stl_files_to_code
{
    VectorOfVectorOfPoints ret;
    ret.reserve(12);
    VectorOfPoints v;

    v.resize(3);
    v[0] = EPoint(-0.5,-0.5,1);
    v[1] = EPoint(0.5,-0.5,1);
    v[2] = EPoint(0.5,0.5,1);
    ret.push_back(v);

    v[0] = EPoint(-0.5,-0.5,1);
    v[1] = EPoint(0.5,0.5,1);
    v[2] = EPoint(-0.5,0.5,1);
    ret.push_back(v);

    v[0] = EPoint(0.5,-0.5,0);
    v[1] = EPoint(-0.5,-0.5,0);
    v[2] = EPoint(-0.5,0.5,0);
    ret.push_back(v);

    v[0] = EPoint(0.5,-0.5,0);
    v[1] = EPoint(-0.5,0.5,0);
    v[2] = EPoint(0.5,0.5,0);
    ret.push_back(v);

    v[0] = EPoint(-0.5,-0.5,0);
    v[1] = EPoint(-0.5,-0.5,1);
    v[2] = EPoint(-0.5,0.5,1);
    ret.push_back(v);

    v[0] = EPoint(-0.5,-0.5,0);
    v[1] = EPoint(-0.5,0.5,1);
    v[2] = EPoint(-0.5,0.5,0);
    ret.push_back(v);

    v[0] = EPoint(0.5,-0.5,1);
    v[1] = EPoint(0.5,-0.5,0);
    v[2] = EPoint(0.5,0.5,0);
    ret.push_back(v);

    v[0] = EPoint(0.5,-0.5,1);
    v[1] = EPoint(0.5,0.5,0);
    v[2] = EPoint(0.5,0.5,1);
    ret.push_back(v);

    v[0] = EPoint(-0.5,-0.5,0);
    v[1] = EPoint(0.5,-0.5,0);
    v[2] = EPoint(0.5,-0.5,1);
    ret.push_back(v);

    v[0] = EPoint(-0.5,-0.5,0);
    v[1] = EPoint(0.5,-0.5,1);
    v[2] = EPoint(-0.5,-0.5,1);
    ret.push_back(v);

    v[0] = EPoint(-0.5,0.5,1);
    v[1] = EPoint(0.5,0.5,1);
    v[2] = EPoint(0.5,0.5,0);
    ret.push_back(v);

    v[0] = EPoint(-0.5,0.5,1);
    v[1] = EPoint(0.5,0.5,0);
    v[2] = EPoint(-0.5,0.5,0);
    ret.push_back(v);

    return ret;
}


VectorOfVectorOfPoints L()
{
    VectorOfVectorOfPoints ret;
    ret.reserve(20);
    VectorOfPoints v;
    v.resize(3);

    // y,z plane, looking towards x>0
    v[0]=EPoint(0,0,0);v[1]=EPoint(0,1,-1);v[2]=EPoint(0,0,-1);ret.push_back(v);
    v[0]=EPoint(0,0,0);v[1]=EPoint(0,1,0);v[2]=EPoint(0,1,-1);ret.push_back(v);
    v[0]=EPoint(0,0,-1);v[1]=EPoint(0,1,-2);v[2]=EPoint(0,0,-2);ret.push_back(v);
    v[0]=EPoint(0,0,-1);v[1]=EPoint(0,1,-1);v[2]=EPoint(0,1,-2);ret.push_back(v);
    v[0]=EPoint(0,1,0);v[1]=EPoint(0,2,-1);v[2]=EPoint(0,1,-1);ret.push_back(v);
    v[0]=EPoint(0,1,0);v[1]=EPoint(0,2,0);v[2]=EPoint(0,2,-1);ret.push_back(v);

    // y,z plane, looking towards x<0
    v[0]=EPoint(1,0,-2);v[1]=EPoint(1,1,-2);v[2]=EPoint(1,0,-1);ret.push_back(v);
    v[0]=EPoint(1,1,-2);v[1]=EPoint(1,1,-1);v[2]=EPoint(1,0,-1);ret.push_back(v);
    v[0]=EPoint(1,0,-1);v[1]=EPoint(1,1,-1);v[2]=EPoint(1,0,0);ret.push_back(v);
    v[0]=EPoint(1,1,-1);v[1]=EPoint(1,1,0);v[2]=EPoint(1,0,0);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(1,1,-1);v[2]=EPoint(1,2,-1);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(1,2,-1);v[2]=EPoint(1,2,0);ret.push_back(v);

    // x,z plane, looking towards y<0
    v[0]=EPoint(0,2,0);v[1]=EPoint(1,2,0);v[2]=EPoint(1,2,-1);ret.push_back(v);
    v[0]=EPoint(0,2,0);v[1]=EPoint(1,2,-1);v[2]=EPoint(0,2,-1);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(1,1,-1);v[2]=EPoint(1,1,-2);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(1,1,-2);v[2]=EPoint(0,1,-2);ret.push_back(v);

    // x,y plane, looking towards z>0
    v[0]=EPoint(0,0,-2);v[1]=EPoint(1,1,-2);v[2]=EPoint(1,0,-2);ret.push_back(v);
    v[0]=EPoint(0,0,-2);v[1]=EPoint(0,1,-2);v[2]=EPoint(1,1,-2);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(1,2,-1);v[2]=EPoint(1,1,-1);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(0,2,-1);v[2]=EPoint(1,2,-1);ret.push_back(v);

    // x,z plane, looking towards y>0
    v[0]=EPoint(1,0,0);v[1]=EPoint(0,0,-1);v[2]=EPoint(1,0,-1);ret.push_back(v);
    v[0]=EPoint(1,0,0);v[1]=EPoint(0,0,0);v[2]=EPoint(0,0,-1);ret.push_back(v);
    v[0]=EPoint(1,0,-1);v[1]=EPoint(0,0,-2);v[2]=EPoint(1,0,-2);ret.push_back(v);
    v[0]=EPoint(1,0,-1);v[1]=EPoint(0,0,-1);v[2]=EPoint(0,0,-2);ret.push_back(v);

    // x,y plane, looking towards z<0
    v[0]=EPoint(1,0,0);v[1]=EPoint(0,1,0);v[2]=EPoint(0,0,0);ret.push_back(v);
    v[0]=EPoint(1,0,0);v[1]=EPoint(1,1,0);v[2]=EPoint(0,1,0);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(0,2,0);v[2]=EPoint(0,1,0);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(1,2,0);v[2]=EPoint(0,2,0);ret.push_back(v);

    return ret;
}

VectorOfVectorOfPoints U()
{
    VectorOfVectorOfPoints ret;
    ret.reserve(20);
    VectorOfPoints v;
    v.resize(3);

    // y,z plane, looking towards x>0
    v[0]=EPoint(0,0,0);v[1]=EPoint(0,1,0);v[2]=EPoint(0,1,-1);ret.push_back(v);
    v[0]=EPoint(0,0,0);v[1]=EPoint(0,1,-1);v[2]=EPoint(0,0,-1);ret.push_back(v);
    v[0]=EPoint(0,0,-1);v[1]=EPoint(0,1,-1);v[2]=EPoint(0,1,-2);ret.push_back(v);
    v[0]=EPoint(0,0,-1);v[1]=EPoint(0,1,-2);v[2]=EPoint(0,0,-2);ret.push_back(v);
    v[0]=EPoint(0,1,0);v[1]=EPoint(0,2,0);v[2]=EPoint(0,2,-1);ret.push_back(v);
    v[0]=EPoint(0,1,0);v[1]=EPoint(0,2,-1);v[2]=EPoint(0,1,-1);ret.push_back(v);
    v[0]=EPoint(0,2,0);v[1]=EPoint(0,3,0);v[2]=EPoint(0,3,-1);ret.push_back(v);
    v[0]=EPoint(0,2,0);v[1]=EPoint(0,3,-1);v[2]=EPoint(0,2,-1);ret.push_back(v);
    v[0]=EPoint(0,2,-1);v[1]=EPoint(0,3,-1);v[2]=EPoint(0,3,-2);ret.push_back(v);
    v[0]=EPoint(0,2,-1);v[1]=EPoint(0,3,-2);v[2]=EPoint(0,2,-2);ret.push_back(v);

    // y,z plane, looking towards x<0
    v[0]=EPoint(1,3,0);v[1]=EPoint(1,2,0);v[2]=EPoint(1,2,-1);ret.push_back(v);
    v[0]=EPoint(1,3,0);v[1]=EPoint(1,2,-1);v[2]=EPoint(1,3,-1);ret.push_back(v);
    v[0]=EPoint(1,3,-1);v[1]=EPoint(1,2,-1);v[2]=EPoint(1,2,-2);ret.push_back(v);
    v[0]=EPoint(1,3,-1);v[1]=EPoint(1,2,-2);v[2]=EPoint(1,3,-2);ret.push_back(v);
    v[0]=EPoint(1,2,0);v[1]=EPoint(1,1,0);v[2]=EPoint(1,1,-1);ret.push_back(v);
    v[0]=EPoint(1,2,0);v[1]=EPoint(1,1,-1);v[2]=EPoint(1,2,-1);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(1,0,0);v[2]=EPoint(1,0,-1);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(1,0,-1);v[2]=EPoint(1,1,-1);ret.push_back(v);
    v[0]=EPoint(1,1,-1);v[1]=EPoint(1,0,-1);v[2]=EPoint(1,0,-2);ret.push_back(v);
    v[0]=EPoint(1,1,-1);v[1]=EPoint(1,0,-2);v[2]=EPoint(1,1,-2);ret.push_back(v);

    // x,z plane, looking towards y<0
    v[0]=EPoint(0,1,-1);v[1]=EPoint(1,1,-1);v[2]=EPoint(1,1,-2);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(1,1,-2);v[2]=EPoint(0,1,-2);ret.push_back(v);
    v[0]=EPoint(0,2,-1);v[2]=EPoint(1,2,-1);v[1]=EPoint(1,2,-2);ret.push_back(v);
    v[0]=EPoint(0,2,-1);v[2]=EPoint(1,2,-2);v[1]=EPoint(0,2,-2);ret.push_back(v);


    // x,y plane, looking towards z>0
    v[0]=EPoint(0,0,-2);v[1]=EPoint(1,1,-2);v[2]=EPoint(1,0,-2);ret.push_back(v);
    v[0]=EPoint(0,0,-2);v[1]=EPoint(0,1,-2);v[2]=EPoint(1,1,-2);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(1,2,-1);v[2]=EPoint(1,1,-1);ret.push_back(v);
    v[0]=EPoint(0,1,-1);v[1]=EPoint(0,2,-1);v[2]=EPoint(1,2,-1);ret.push_back(v);
    v[0]=EPoint(0,2,-2);v[1]=EPoint(1,3,-2);v[2]=EPoint(1,2,-2);ret.push_back(v);
    v[0]=EPoint(0,2,-2);v[1]=EPoint(0,3,-2);v[2]=EPoint(1,3,-2);ret.push_back(v);

    // x,z plane, looking towards y>0
    v[0]=EPoint(1,0,0);v[1]=EPoint(0,0,0);v[2]=EPoint(0,0,-1);ret.push_back(v);
    v[0]=EPoint(1,0,0);v[1]=EPoint(0,0,-1);v[2]=EPoint(1,0,-1);ret.push_back(v);
    v[0]=EPoint(1,0,-1);v[1]=EPoint(0,0,-1);v[2]=EPoint(0,0,-2);ret.push_back(v);
    v[0]=EPoint(1,0,-1);v[1]=EPoint(0,0,-2);v[2]=EPoint(1,0,-2);ret.push_back(v);
    v[0]=EPoint(0,3,0);v[1]=EPoint(1,3,0);v[2]=EPoint(1,3,-1);ret.push_back(v);
    v[0]=EPoint(0,3,0);v[1]=EPoint(1,3,-1);v[2]=EPoint(0,3,-1);ret.push_back(v);
    v[0]=EPoint(0,3,-1);v[1]=EPoint(1,3,-1);v[2]=EPoint(1,3,-2);ret.push_back(v);
    v[0]=EPoint(0,3,-1);v[1]=EPoint(1,3,-2);v[2]=EPoint(0,3,-2);ret.push_back(v);

    // x,y plane, looking towards z<0
    v[0]=EPoint(1,0,0);v[1]=EPoint(1,1,0);v[2]=EPoint(0,1,0);ret.push_back(v);
    v[0]=EPoint(1,0,0);v[1]=EPoint(0,1,0);v[2]=EPoint(0,0,0);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(1,2,0);v[2]=EPoint(0,2,0);ret.push_back(v);
    v[0]=EPoint(1,1,0);v[1]=EPoint(0,2,0);v[2]=EPoint(0,1,0);ret.push_back(v);
    v[0]=EPoint(1,2,0);v[1]=EPoint(1,3,0);v[2]=EPoint(0,3,0);ret.push_back(v);
    v[0]=EPoint(1,2,0);v[1]=EPoint(0,3,0);v[2]=EPoint(0,2,0);ret.push_back(v);

    return ret;
}
