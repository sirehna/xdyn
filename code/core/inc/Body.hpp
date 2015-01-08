/*
 * Body.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef BODY_HPP_
#define BODY_HPP_

#include "BodyStates.hpp"
struct YamlBody;

class Body
{
    public:
        Body();
        /** \brief Use SurfaceElevation to compute wave height & update accordingly
         */
        void update_intersection_with_free_surface(const EnvironmentAndFrames& env,
                                                   const double t
                                                  );

        BodyStates states;
};

#endif /* BODY_HPP_ */
