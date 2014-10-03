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

        /**  \brief Computes the relative wave height at a given point.
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

        /**  \brief Computes the relative wave height for a matrix of Points.
          *  \returns zwave - z for each point.
          *  \snippet hydro_model/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_matrix_example
          */
        std::vector<double> get_relative_wave_height(const ssc::kinematics::PointMatrixPtr& P,                     //!< Points for which to compute the relative wave height
                                                     const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                                     const double t //!< Current instant (in seconds)
                                                    ) const;

        /**  \brief Computes the dynamic pressure at a given point.
          *  \details The input point P can be projected into any reference
          *           frame: this method will request a transform from a
          *           Kinematics object to express it in the NED frame.
          *  \returns Pdyn (in Pascal)
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_example
          */
        double get_dynamic_pressure(const double rho, // Water density (in kg/m^3)
                                    const double g, //!< Gravity (in m/s^2)
                                    const ssc::kinematics::Point& P, //!< Position of point P, relative to the centre of the NED frame, but projected in any frame
                                    const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k, //!< Object used to compute the transforms to the NED frame
                                    const double t  //!< Current instant (in seconds)
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
        /**  \brief Relative surface elevation
          *  \returns Difference between the height of a point & the elevation of that point at a given instant, in meters.
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 29
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.20, page 125
          *  \returns zwave - z
          */
        virtual double wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   const double z, //!< z-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                                   const double t //!< Current instant (in seconds)
                                   ) const = 0;

        /**  \brief Unsteady pressure field induced by undisturbed waves. Used to compute the Froude-Krylov forces.
          *  \details Also called "subsurface pressure" (DNV), "unsteady pressure" (Faltinsen) or constant pressure contour (Lloyd)
          *           The dynamic pressure is in fact one of the terms of Bernoulli's equation, which can be derived from the conservation
          *           of energy for a fluid in motion.
          *           \f[\int_C \frac{\partial \Phi_I(x,y,z,t)}{\partial t}\f]
          *  \returns Dynamic pressure in Pascal
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation VI.34, page 183
          *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 68
          */
        virtual double dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                        const double g,   //!< gravity (in m/s^2)
                                        const double x,   //!< x-position in the NED frame (in meters)
                                        const double y,   //!< y-position in the NED frame (in meters)
                                        const double z,   //!< z-position in the NED frame (in meters)
                                        const double t    //!< Current time instant (in seconds)
                                        ) const = 0;

        /**  \brief If the wave output mesh is not defined in NED, use Kinematics to update its x-y coordinates
          */
        TR1(shared_ptr)<ssc::kinematics::PointMatrix> get_output_mesh_in_NED_frame(const TR1(shared_ptr)<ssc::kinematics::Kinematics>& k //!< Object used to compute the transforms to the NED frame
                                            ) const;

        TR1(shared_ptr)<ssc::kinematics::PointMatrix> output_mesh; //!< Mesh defined in the 'output' section of the YAML file. Points at which we want to know the wave height at each instant
};

#endif /* SURFACELEVATIONINTERFACE_HPP_ */
