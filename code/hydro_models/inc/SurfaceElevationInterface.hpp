/*
 * SurfaceElevationInterface.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef SURFACELEVATIONINTERFACE_HPP_
#define SURFACELEVATIONINTERFACE_HPP_

#include "tr1_macros.hpp"
#include TR1INC(memory)
#include <vector>

class Point;
class Kinematics;
class PointMatrix;

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
class SurfaceElevationInterface
{
    public:
        SurfaceElevationInterface(const TR1(shared_ptr)<PointMatrix>& output_mesh);

        virtual ~SurfaceElevationInterface();

        /**  \author cec
          *  \date 24 avr. 2014, 10:32:29
          *  \brief Computes the relative wave height at a given point.
          *  \details The input point P can be projected into any reference
          *           frame: this method will request a transform from a
          *           Kinematics object to express it in the NED frame.
          *  \returns zwave - z
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_example
          */
        double get_relative_wave_height(const Point& P, //!< Position of point P, relative to the centre of the NED frame, but projected in any frame
                                        const TR1(shared_ptr)<Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                        const double t //!< Current instant (in seconds)
                                       ) const;

        /**  \author cec
          *  \date 24 avr. 2014, 13:00:47
          *  \brief Computes the relative wave height for a matrix of Points.
          *  \returns zwave - z for each point.
          *  \snippet hydro_model/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_matrix_example
          */
        std::vector<double> get_relative_wave_height(const PointMatrix& P,                     //!< Points for which to compute the relative wave height
                                                     const TR1(shared_ptr)<Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                                     const double t //!< Current instant (in seconds)
                                                    ) const;

        /**  \brief Computes the wave heights at the points given in the 'output' section of the YAML file.
          *  \returns Vector of wave heights (in meters) for each point in output_mesh
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest method_example
          */
        std::vector<double> get_waves_on_mesh(const TR1(shared_ptr)<Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                              const double t //<! Current instant (in seconds)
                                             ) const;

    private:
        /**  \author cec
          *  \date 24 avr. 2014, 10:29:58
          *  \brief Wave model interface.
          *  \details This method will be called by the public get_relative_wave_height method.
          *  \returns zwave - z
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest wave_height_example
          */
        virtual double wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   const double z, //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   const double t //!< Current instant (in seconds)
                                   ) const = 0;

        TR1(shared_ptr)<PointMatrix> output_mesh; //!< Mesh defined in the 'output' section of the YAML file. Points at which we want to know the wave height at each instant
};

#endif /* SURFACELEVATIONINTERFACE_HPP_ */
