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
}

#endif /* HYDROSTATIC_HPP_ */
