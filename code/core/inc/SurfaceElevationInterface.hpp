/*
 * SurfaceElevationInterface.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef SURFACELEVATIONINTERFACE_HPP_
#define SURFACELEVATIONINTERFACE_HPP_

#include "GeometricTypes3d.hpp"
#include "SurfaceElevationGrid.hpp"
#include "Observer.hpp"
#include <ssc/kinematics.hpp>
#include <ssc/macros/tr1_macros.hpp>
#include TR1INC(memory)

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
        SurfaceElevationInterface(
                const ssc::kinematics::PointMatrixPtr& output_mesh,
                const std::pair<std::size_t,std::size_t>& output_mesh_size = std::make_pair((std::size_t)0,(std::size_t)0)
        );

        virtual ~SurfaceElevationInterface();

        /**  \brief Computes surface elevation for each point on mesh.
          *  \details Updates the absolute surface elevation & the relative wave height.
          */
        void update_surface_elevation(const ssc::kinematics::PointMatrixPtr& M,     //!< Points for which to compute the relative wave height
                                      const ssc::kinematics::KinematicsPtr& k,      //!< Object used to compute the transforms to the NED frame
                                      const double t                                //!< Current instant (in seconds)
                                     );

        /**  \brief Returns the relative wave height computed by update_surface_elevation
          *  \returns zwave - z for each point in mesh.
          *  \snippet hydro_model/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_matrix_example
          */
        std::vector<double> get_relative_wave_height() const;

        /**  \brief Returns the absolute wave height (z coordinate in NED frame) computed by update_surface_elevation
          *  \returns zwave for each point (x,y) in mesh.
          */
        std::vector<double> get_surface_elevation() const;

        /**  \brief Returns the pair of number of points describing the surface elevation mesh
          *  \returns pair (nx,ny)
          */
        std::pair<std::size_t,std::size_t> get_output_mesh_size() const
        {
            return output_mesh_size;
        }

        /**  \brief Calculate radiation forces using first order force RAO
          *  \returns Force and torque
          */
        double evaluate_rao(const double x, //!< x-position of the RAO's calculation point in the NED frame (in meters)
                            const double y, //!< y-position of the RAO's calculation point in the NED frame (in meters)
                            const double t, //!< Current time instant (in seconds)
                            const std::vector<std::vector<double> >& rao_module, //!< Module of the RAO
                            const std::vector<std::vector<double> >& rao_phase //!< Phase of the RAO
                            ) const;

        /**  \brief Computes the orbital velocity at given points.
          *  \returns Velocity of the fluid at given points & instant, in m/s
          */
        ssc::kinematics::PointMatrix get_orbital_velocity(const double g,                //!< gravity (in m/s^2)
                                                          const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
                                                          const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
                                                          const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
                                                          const double t,                //!< Current time instant (in seconds)
                                                          const std::vector<double>& eta //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                         ) const;
        
        virtual std::vector<std::vector<double> > get_wave_directions_for_each_model() const;
        virtual std::vector<std::vector<double> > get_wave_angular_frequency_for_each_model() const;

        /**  \brief Computes the dynamic pressure at given points.
          *  \details The input point matrix P can be projected into any reference
          *           frame: this method will request a transform from a
          *           Kinematics object to express it in the NED frame.
          *  \returns Pdyn (in Pascal)
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest get_relative_wave_height_example
          */
        std::vector<double> get_dynamic_pressure(const double rho,                        //!< Water density (in kg/m^3)
                                                 const double g,                          //!< Gravity (in m/s^2)
                                                 const ssc::kinematics::PointMatrix& P,   //!< Positions of points P, relative to the centre of the NED frame, but projected in any frame
                                                 const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
                                                 const std::vector<double>& eta,          //!< Wave elevation at P in the NED frame (in meters)
                                                 const double t                           //!< Current instant (in seconds)
                                                 ) const;

        /**  \brief Computes the wave heights at the points given in the 'output' section of the YAML file.
          *  \returns Vector of coordinates on the free surface (in the NED frame),
          *           the z coordinate being the wave height (in meters), for each
          *           point in output_mesh
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest method_example
          */
        ssc::kinematics::PointMatrix get_waves_on_mesh(
                const ssc::kinematics::KinematicsPtr& k, //!< Object used to compute the transforms to the NED frame
                const double t                           //!< Current instant (in seconds)
                ) const;

        /**  \brief Computes the wave heights at the points given in the 'output' section of the YAML file.
          *  \returns a structure containing vector \a x, vector \a y and
          *           matrix \a z where
          *           \li \a x gives the X-variation of the mesh
          *           \li \a y gives the Y-variation of the mesh
          *           \li \a z gives the associated free surface elevation in
          *           the NED frame.
          */
        SurfaceElevationGrid get_waves_on_mesh_as_a_grid(
                const ssc::kinematics::KinematicsPtr& k,    //!< Object used to compute the transforms to the NED frame
                const double t                              //!< Current instant (in seconds)
                ) const;

        /**  \brief Computes the wave heights on a mesh. Used by get_waves_on_mesh
          *  \returns Vector of coordinates on the free surface (in the NED frame),
          *           the z coordinate being the wave height (in meters), for each
          *           point in P
          *  \snippet hydro_models/unit_tests/src/WaveModelInterfaceTest.cpp WaveModelInterfaceTest method_example
          */
        ssc::kinematics::PointMatrix get_points_on_free_surface(
                const double t,                               //!< Current instant (in seconds)
                const ssc::kinematics::PointMatrixPtr& Mned   //!< Output mesh in NED frame
                ) const;

        /**  \brief Computes the surface elevation at given points.
          *  \returns Surface elevations of a list of points at a given instant, in meters.
          *  \returns zwave - z
          */
        std::vector<double> get_wave_height(const std::vector<double> &x, //!< x-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
                                            const std::vector<double> &y, //!< y-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
                                            const double t                //!< Current instant (in seconds)
                                           ) const;

        virtual void serialize_wave_spectra_before_simulation(ObserverPtr& observer) const;

        /**  \brief Surface elevation
              *  \returns Surface elevations of a list of points at a given instant, in meters.
              *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
              *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
              *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 29
              *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.20, page 125
              *  \returns zwave - z
              */
        virtual std::vector<double> wave_height(const std::vector<double> &x, //!< x-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
                                                const std::vector<double> &y, //!< y-coordinates of the points, relative to the centre of the NED frame, projected in the NED frame
                                                const double t                //!< Current instant (in seconds)
                                                ) const = 0;

        /**  \author cec
          *  \date Feb 3, 2015, 10:06:45 AM
          *  \brief Orbital velocity
          *  \returns Velocity of the fluid at given points & instant, in m/s
          */
        virtual ssc::kinematics::PointMatrix orbital_velocity(const double g,                //!< gravity (in m/s^2)
                                                              const std::vector<double>& x,  //!< x-positions in the NED frame (in meters)
                                                              const std::vector<double>& y,  //!< y-positions in the NED frame (in meters)
                                                              const std::vector<double>& z,  //!< z-positions in the NED frame (in meters)
                                                              const double t,                //!< Current time instant (in seconds)
                                                              const std::vector<double>& eta //!< Wave elevations at (x,y) in the NED frame (in meters)
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
        virtual std::vector<double> dynamic_pressure(const double rho,               //!< water density (in kg/m^3)
                                                     const double g,                 //!< gravity (in m/s^2)
                                                     const std::vector<double> &x,   //!< x-positions in the NED frame (in meters)
                                                     const std::vector<double> &y,   //!< y-positions in the NED frame (in meters)
                                                     const std::vector<double> &z,   //!< z-positions in the NED frame (in meters)
                                                     const std::vector<double> &eta, //!< Wave elevations at (x,y) in the NED frame (in meters)
                                                     const double t                  //!< Current time instant (in seconds)
                                                     ) const = 0;

        virtual std::vector<FlatDiscreteDirectionalWaveSpectrum> get_flat_directional_spectra(const double x, const double y, const double t) const = 0;
        virtual std::vector<DiscreteDirectionalWaveSpectrum> get_directional_spectra(const double x, const double y, const double t) const = 0;
        /**  \brief If the wave output mesh is not defined in NED, use Kinematics to update its x-y coordinates
          */

    private:
        ssc::kinematics::PointMatrixPtr get_output_mesh_in_NED_frame(const ssc::kinematics::KinematicsPtr& k //!< Object used to compute the transforms to the NED frame
                                                                    ) const;

        ssc::kinematics::PointMatrixPtr output_mesh;            //!< Mesh defined in the 'output' section of the YAML file. Points at which we want to know the wave height at each instant
        std::pair<std::size_t,std::size_t> output_mesh_size;    //!< Mesh size defined as a pair containing nx and ny
        std::vector<double> relative_wave_height_for_each_point_in_mesh;
        std::vector<double> surface_elevation_for_each_point_in_mesh;
};

typedef TR1(shared_ptr)<SurfaceElevationInterface> SurfaceElevationPtr;

#endif /* SURFACELEVATIONINTERFACE_HPP_ */
