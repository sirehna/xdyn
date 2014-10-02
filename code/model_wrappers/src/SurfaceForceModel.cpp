/*
 * SurfaceForceModel.cpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#include "Body.hpp"
#include "SurfaceForceModel.hpp"

SurfaceForceModel::SurfaceForceModel(const EnvironmentAndFrames& env_) :
        env(env_),
        g_in_NED(ssc::kinematics::Point("NED", 0, 0, env.g))
{
}

SurfaceForceModel::~SurfaceForceModel()
{
}

ssc::kinematics::Wrench SurfaceForceModel::operator()(const Body& body, const double t) const
{
    const ssc::kinematics::RotationMatrix ned2mesh = env.k->get("NED", std::string("mesh(") + body.name + ")").get_rot();
    ssc::kinematics::UnsafeWrench F(body.G);
    const EPoint g_in_mesh(ned2mesh*g_in_NED.v);
    const double orientation_factor = body.intersector->mesh->orientation_factor;
    for (auto that_facet = begin(body.intersector) ; that_facet != end(body.intersector) ; ++that_facet)
    {
        const DF f = dF(that_facet, body.intersector, env, body, t);
        F+= ssc::kinematics::UnsafeWrench(body.G, orientation_factor*f.dF, orientation_factor*(f.C-body.G.v).cross(f.dF));
    }
    return F;
}
