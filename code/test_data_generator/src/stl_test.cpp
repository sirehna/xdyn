#include "GeometricTypes3d.hpp"

VectorOfVectorOfPoints stl0();
VectorOfVectorOfPoints stl0()
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

VectorOfVectorOfPoints stl1();
VectorOfVectorOfPoints stl1()
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
