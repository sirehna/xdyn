#ifndef MESHNUMERIC_HPP
#define MESHNUMERIC_HPP

#include <math.h>
#include <limits>

#ifndef MESH_EPSILON
#define MESH_EPSILON     (100*std::numeric_limits<double>::epsilon())
#endif

/** \def MESH_EQ(a,b)*/
#ifndef MESH_EQ
#define MESH_EQ(a, b)               ((a)==(b))
#endif

/** \def MESH_LT(a,b) Lower than (strict)*/
#ifndef MESH_LT
#define MESH_LT(a, b)            (((b) - (a)) > 0)
#endif

#endif
