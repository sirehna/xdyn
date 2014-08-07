/*
 * ListOfWaveModels.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef LISTOFWAVEMODELS_HPP_
#define LISTOFWAVEMODELS_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)

#include <vector>

class WaveModel;

/** \author cec
 *  \date Aug 7, 2014, 7:45:56 AM
 *  \brief Stores several wave directional spectra with potentially different
 *         models (Airy, Stokes II...)
 *  \details Just sums the results of each model
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet wave_models/unit_tests/src/ListOfWaveModelsTest.cpp ListOfWaveModelsTest example
 *  \section ex2 Expected output
 *  \snippet wave_models/unit_tests/src/ListOfWaveModelsTest.cpp ListOfWaveModelsTest expected output
 */
class ListOfWaveModels
{
    public:
        ListOfWaveModels(const std::vector<TR1(shared_ptr)<WaveModel> >& models);

        /**  \author cec
          *  \date Aug 7, 2014, 7:47:40 AM
          *  \brief Surface elevation
          *  \returns Elevation of a point at a given instant, in meters.
          *  \snippet wave_models/unit_tests/src/ListOfWaveModelsTest.cpp ListOfWaveModelsTest method_example
          */
        double elevation(const double x,//!< x-position in the NED frame (in meters)
                         const double y,//!< y-position in the NED frame (in meters)
                         const double t //!< Current time instant (in seconds)
                         ) const;

    private:
        ListOfWaveModels(); // Disabled
        std::vector<TR1(shared_ptr)<WaveModel> > models;
};

#endif /* LISTOFWAVEMODELS_HPP_ */
