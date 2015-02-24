/*
 * CenterOfMass.hpp
 *
 *  Created on: Oct 14, 2014
 *      Author: cady
 */

#ifndef CENTEROFMASS_HPP_
#define CENTEROFMASS_HPP_

#include "GeometricTypes3d.hpp"

struct CenterOfMass
{
    CenterOfMass(const EPoint& G_, const double v_);
    CenterOfMass& operator+=(const CenterOfMass& com);

    EPoint G;
    double volume;
    bool all_facets_are_in_same_plane;

    private:
        CenterOfMass();
};

#endif /* CENTEROFMASS_HPP_ */
