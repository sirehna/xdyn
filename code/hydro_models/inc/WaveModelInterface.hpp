/*
 * WaveModelInterface.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef WAVEMODELINTERFACE_HPP_
#define WAVEMODELINTERFACE_HPP_

#include <tr1/memory>

class Point;
class Kinematics;

/** \author cec
 *  \date 24 avr. 2014, 10:28:25
 *  \brief Interface to wave models
 *  \details It is this base class' responsibility to change the reference frame of the input point.
 *  \ingroup hydro_models
 *  \section ex1 Example
 *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest example
 *  \section ex2 Expected output
 *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest expected output
 */
class WaveModelInterface
{
    public:
        WaveModelInterface(const std::tr1::shared_ptr<Kinematics>& k //!< Object used to compute the transforms to the NED frame
                          );

        virtual ~WaveModelInterface();

        /**  \author cec
          *  \date 24 avr. 2014, 10:32:29
          *  \brief Computes the relative wave height at a given point.
          *  \details The input point P can be projected into any reference
          *           frame: this method will request a transform from a
          *           Kinematics object to express it in the NED frame.
          *  \returns zwave - z
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_example
          */
        double get_relative_wave_height(const Point& P //!< Position of point P, relative to the centre of the NED frame, but projected in any frame
                                       ) const;

    private:
        WaveModelInterface(); // Disabled because we must have a Kinematics object to do anything

        /**  \author cec
          *  \date 24 avr. 2014, 10:29:58
          *  \brief Wave model interface.
          *  \details This method will be called by the public get_relative_wave_height method.
          *  \returns zwave - z
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest wave_height_example
          */
        virtual double wave_height(double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   double z  //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   ) const = 0;

        const std::tr1::shared_ptr<Kinematics>& k;
};

#endif /* WAVEMODELINTERFACE_HPP_ */
