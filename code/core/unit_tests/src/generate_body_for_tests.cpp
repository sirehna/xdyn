/*
 * simulator_test.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "generate_body_for_tests.hpp"
#include "YamlRotation.hpp"
#include "BodyBuilder.hpp"
#include "TriMeshTestData.hpp"

BodyPtr get_body(const std::string& name)
{
    return get_body(name, two_triangles());
}

BodyPtr get_body(const std::string& name, const VectorOfVectorOfPoints& points)

{
    YamlRotation rot;
    rot.convention.push_back("z");
    rot.convention.push_back("y'");
    rot.convention.push_back("x''");
    rot.order_by = "angle";
    return BodyBuilder(rot).build(name, points, 0, 0, rot, true);
}
