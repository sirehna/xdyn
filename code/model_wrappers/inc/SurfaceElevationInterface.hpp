/*
 * SurfaceElevationInterface.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef SURFACELEVATIONINTERFACE_HPP_
#define SURFACELEVATIONINTERFACE_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)
#include <vector>

#include "GeometricTypes3d.hpp"
#include <ssc/kinematics.hpp>

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
        SurfaceElevationInterface(const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& output_mesh);

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
        double get_relative_wave_height(const ssc::kinematics::Point& P, //!< Position of point P, relative to the centre of the NED frame, but projected in any frame
                                        const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                        const double t //!< Current instant (in seconds)
                                       ) const;

        /**  \author cec
          *  \date 24 avr. 2014, 13:00:47
          *  \brief Computes the relative wave height for a matrix of Points.
          *  \returns zwave - z for each point.
          *  \snippet hydro_model/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_matrix_example
          */
        std::vector<double> get_relative_wave_height(const ssc::kinematics::PointMatrix& P,                     //!< Points for which to compute the relative wave height
                                                     const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                                     const double t //!< Current instant (in seconds)
                                                    ) const;

        /**  \brief Computes the wave heights at the points given in the 'output' section of the YAML file.
          *  \returns Vector of coordinates on the free surface (in the NED frame),
          *           the z coordinate being the wave height (in meters), for each
          *           point in output_mesh
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest method_example
          */
        std::vector<ssc::kinematics::Point> get_waves_on_mesh(const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                              const double t //<! Current instant (in seconds)
                                             ) const;

        /**  \brief Computes the wave heights on a mesh. Used by get_waves_on_mesh
          *  \returns Vector of coordinates on the free surface (in the NED frame),
          *           the z coordinate being the wave height (in meters), for each
          *           point in P
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest method_example
          */
        std::vector<ssc::kinematics::Point> get_points_on_free_surface(const double t,                          //<! Current instant (in seconds)
                                                      const TR1(shared_ptr)<ssc::kinematics::PointMatrix>& Mned //!< Output mesh in NED frame
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

        /**  \brief If the wave output mesh is not defined in NED, use Kinematics to update its x-y coordinates
          */
        TR1(shared_ptr)<ssc::kinematics::PointMatrix> get_output_mesh_in_NED_frame(const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k //!< Object used to compute the transforms to the NED frame
                                            ) const;

        TR1(shared_ptr)<ssc::kinematics::PointMatrix> output_mesh; //!< Mesh defined in the 'output' section of the YAML file. Points at which we want to know the wave height at each instant
};

#endif /* SURFACELEVATIONINTERFACE_HPP_ */
