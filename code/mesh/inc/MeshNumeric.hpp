#ifndef MESHNUMERIC_HPP
#define MESHNUMERIC_HPP

#include <math.h>
#include <limits>

#ifndef MESH_COMPUTE_EPSILON
#define MESH_COMPUTE_EPSILON     std::numeric_limits<double>::epsilon()
#endif

/** \def MESH_EQ(a,b)*/
#ifndef MESH_EQ
#define MESH_EQ(a, b)               (fabs((a) - (b)) <= (MESH_COMPUTE_EPSILON))
#endif

/** \def MESH_LT(a,b) Lower than (strict)*/
#ifndef MESH_LT
#define MESH_LT(a, b)            (((b) - (a)) > (MESH_COMPUTE_EPSILON))
#endif

#endif
