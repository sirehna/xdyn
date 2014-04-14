#include <string>
#include "TriMesh.hpp"

/**
 *\brief reads an ASCII STL (stereolithography) file.
<<<<<<< HEAD
 * \input[in] input String containing the STL data
 * \return Raw list of unmerged triangles
 * \note
 * An STL file describes a raw unstructured triangulated surface by the unit
 * normal and vertices (ordered by the right-hand rule) of the triangles using
 * a three-dimensional Cartesian coordinate system
 * \verbatim
=======
 *
 * \input[in] input String containing the STL data
 * \return Raw list of unmerged triangles
 * \note:
 *   \verbatim
>>>>>>> Added the read of a ASCII STL file
 *    solid MYSOLID
 *      facet normal 0.4 0.4 0.2
 *        outerloop
 *          vertex  1.0 2.1 3.2
 *          vertex  2.1 3.7 4.5
 *          vertex  3.1 4.5 6.7
 *        endloop
 *      endfacet
<<<<<<< HEAD
 *      .
 *      .
 *      .
=======
 *      ...
>>>>>>> Added the read of a ASCII STL file
 *      facet normal 0.2 0.2 0.4
 *        outerloop
 *          vertex  2.0 2.3 3.4
 *          vertex  3.1 3.2 6.5
 *          vertex  4.1 5.5 9.0
 *        endloop
 *      endfacet
 *    endsolid MYSOLID
 * \endverbatim
 *  Reference:
 *
 *    3D Systems, Inc,
 *    Stereolithography Interface Specification,
 *    October 1989.
 */
VectorOfPoint3dTriplet read_stl(const std::string& input);
