/*
 * BodyWithoutSurfaceForces.hpp
 *
 *  Created on: Jan 9, 2015
 *      Author: cady
 */

#ifndef BODYWITHOUTSURFACEFORCES_HPP_
#define BODYWITHOUTSURFACEFORCES_HPP_

#include "Body.hpp"

class BodyWithoutSurfaceForces : public Body
{
    public:
        BodyWithoutSurfaceForces(const size_t idx);
        void update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                                           const double t
                                                          );
};

#endif /* BODYWITHOUTSURFACEFORCES_HPP_ */
