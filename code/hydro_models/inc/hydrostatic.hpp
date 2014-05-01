/*
 * hydrostatic.hpp
 *
 *  Created on: Apr 30, 2014
 *      Author: cady
 */

#ifndef HYDROSTATIC_HPP_
#define HYDROSTATIC_HPP_

#include <vector>
#include <cstddef> //size_t
#include <utility> //std::pair
#include <Eigen/Dense>

#include "GeometricTypes3d.hpp"

namespace hydrostatic
{
    /**  \author cec
      *  \date Apr 30, 2014, 8:17:17 AM
      *  \brief Checks if three points are totally immerged or not
      *  \details Needs the indices of those points & a vector of relative heights.
      *  \returns True if all three points are immerged, false otherwise.
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest totally_immerged_example
      */
    bool totally_immerged(const size_t idx[3], //!< Indices to check
                          const std::vector<double>& delta_z //!< Vector of relative wave heights (positive if point is immerged)
                          );

    /**  \author cec
      *  \date Apr 30, 2014, 10:10:40 AM
      *  \brief Comptues average relative immersion of three points
      *  \details Needs the indices of those points & a vector of relative heights.
      *  \returns Average relative immersion
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest average_immersion_example
      */
    double average_immersion(const size_t idx[3], //!< Indices of the points
                             const std::vector<double>& delta_z //!< Vector of relative wave heights (positive if point is immerged)
                            );
    /**  \author cec
      *  \date Apr 30, 2014, 11:03:52 AM
      *  \brief Computes the immerged polygon from a facet
      *  \returns Coordinate matrix (one point per column)
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest immerged_polygon_example
      */
    Eigen::Matrix<double,3,Eigen::Dynamic> immerged_polygon(const Eigen::Matrix<double,3,Eigen::Dynamic>& M, //!< Matrix containing all the points in the mesh
                                                                  const size_t idx[3], //!< Indices of the points
                                                                  const std::vector<double>& v //!< Vector of relative wave heights (positive if point is immerged)
                                                                  );

    /**  \author cec
      *  \date Apr 30, 2014, 11:41:54 AM
      *  \brief Computes the index of the first & the last positive values in a vector
      *  \details This is just a wrapper for std::find_first_of & std::find_last_of but the semantics are clearer in this context if we create a function
      *  \returns Pair of indexes: first is index of first emerged point & second is index of last emerged point.
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest first_and_last_emerged_points_example
      */
    std::pair<size_t,size_t> first_and_last_emerged_points(const std::vector<double>& z //!< List of immersions
                                                          );

    /**  \author cec
      *  \date Apr 30, 2014, 12:40:15 PM
      *  \brief Given two points A, B & their relative immersions, give intersection with free surface
      *  \details The points must not both be emerged or immerged.
      *           This function assumes that the surface is a plane.
      *  \returns A point intersecting (AB) and the free surface
      *  \snippet hydro_models/unit_tests/src/hydrostaticTest.cpp hydrostaticTest intersection_example
      */
    EPoint intersection(const EPoint& A,  //<! First point
                        const double dzA, //<! Relative immersion of first point
                        const EPoint& B,  //<! Second point
                        const double dzB  //<! Relative immersion of second point
                           );

}

#endif /* HYDROSTATIC_HPP_ */
