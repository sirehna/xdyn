/*
 * Stretching.hpp
 *
 *  Created on: Dec 20, 2016
 *      Author: cady
 */

#ifndef STRETCHING_HPP_
#define STRETCHING_HPP_


struct YamlStretching;

/** \author cec
 *  \date Dec 20, 2016, 12:21 PM
 *  \brief Rescale the z-axis with delta stretching model (used to compute orbital velocities)
 *  \details Prevents non-physical orbital velocities above the mean surface level
 *           The function is \f$z\mapsto z$\f for \f$z>h_{\Delta}$\f and \f$z\mapsto (z-h_{\Delta})\frac{\Delta\eta-h_{\Delta}}{\eta-h_{\Delta}}+h_{\Delta}$\f otherwise
 *  \ingroup wave_models
 *  \section ex1 Example
 *  \snippet environment_models/unit_tests/src/StretchingTest.cpp StretchingTest example
 *  \section ex2 Expected output
 *  \snippet environment_models/unit_tests/src/StretchingTest.cpp StretchingTest expected output
 */
class Stretching
{
    public:
        Stretching(
            const YamlStretching& input //!< Usually read from YAML
            );
        double rescaled_z(
            const double original_z, //!< z value we wish to rescale (in meters)
            const double wave_height //!< Wave height (in meters), z being oriented downwards
            ) const;

    private:
        Stretching(); // Disabled
        double delta; //!< 0 for Wheeler stretching, 1 for linear extrapolation
        double h;     //!< Depth (in meters) over which the stretching is taken into account. Should usually be equal to "depth" (or 0 for no stretching)
};


#endif /* STRETCHING_HPP_ */
