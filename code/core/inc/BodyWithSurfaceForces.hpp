/*
 * BodyWithSurfaceForces.hpp
 *
 *  Created on: Jan 9, 2015
 *      Author: cady
 */

#ifndef BODYWITHSURFACEFORCES_HPP_
#define BODYWITHSURFACEFORCES_HPP_

#include "Body.hpp"

class BodyWithSurfaceForces : public Body
{
    public:
        BodyWithSurfaceForces(const size_t idx);
        BodyWithSurfaceForces(const BodyStates& states, const size_t idx);
        void update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                                           const double t
                                                          );
};

#endif /* BODYWITHSURFACEFORCES_HPP_ */
