#ifndef HOS_HPP_
#define HOS_HPP_

#include "SurfaceElevationInterface.hpp"


/** \author cec
 *  \date 22 Nov 2017, 15:59:25
 *  \brief Interface to ECN's HOS wave model.
 *  \details Connects to HOS via zmq using protobufs
 */
class HOS: public SurfaceElevationInterface
{
    public:
        HOS(const double dummy,
            const std::pair<std::size_t,std::size_t> output_mesh_size = std::make_pair((std::size_t)0,(std::size_t)0),
            const ssc::kinematics::PointMatrixPtr& output_mesh = ssc::kinematics::PointMatrixPtr(new ssc::kinematics::PointMatrix("NED", 0)));

        /**
         *  \section ex1 Example
         *  \snippet hos/unit_tests/src/HOSTest.cpp HOSTest relative_wave_height example
         *  \section ex2 Expected output
         *  \snippet core/unit_tests/src/HOSTest.cpp HOSTest relative_wave_height expected output
         *  */
        double wave_height(const double x, //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double y, //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double t  //!< Current instant (in seconds)
                           ) const;

        /**  \brief Calculate radiation forces using first order force RAO
          *  \returns Force and torque
          */
        double evaluate_rao(const double x, //!< x-position of the RAO's calculation point in the NED frame (in meters)
                            const double y, //!< y-position of the RAO's calculation point in the NED frame (in meters)
                            const double t, //!< Current time instant (in seconds)
                            const std::vector<std::vector<double> >& rao_module, //!< Module of the RAO
                            const std::vector<std::vector<double> >& rao_phase //!< Phase of the RAO
                             ) const;

        /**  \author cec
          *  \date Feb 3, 2015, 10:06:45 AM
          *  \brief Orbital velocity
          *  \returns Velocity of the fluid at a given point & instant, in m/s
         *  \section ex1 Example
         *  \snippet core/unit_tests/src/HOSTest.cpp HOSTest orbital_velocity example
         *  \section ex2 Expected output
         *  \snippet core/unit_tests/src/HOSTest.cpp HOSTest orbital_velocity expected output
          */
        ssc::kinematics::Point orbital_velocity(const double g,   //!< gravity (in m/s^2)
                                                const double x,   //!< x-position in the NED frame (in meters)
                                                const double y,   //!< y-position in the NED frame (in meters)
                                                const double z,   //!< z-position in the NED frame (in meters)
                                                const double t,   //!< z-position in the NED frame (in meters)
                                                const double eta  //!< Wave elevation at (x,y) in the NED frame (in meters)
                                               ) const;

    private:
        HOS(); // Disabled

        /**  \brief Unsteady pressure field induced by undisturbed waves. Used to compute the Froude-Krylov forces.
          *  \details Also called "subsurface pressure" (by DNV), "unsteady pressure" (by Faltinsen) or constant pressure contour (by Lloyd)
          *           The dynamic pressure is in fact one of the terms of Bernoulli's equation, which can be derived from the conservation
          *           of energy for a fluid in motion.
          *           \f[\int_C \frac{\partial \Phi_I(x,y,z,t)}{\partial t}\f]
          *  \returns Dynamic pressure in Pascal
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation VI.34, page 183
          *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 68
         *  \section ex1 Example
         *  \snippet core/unit_tests/src/HOSTest.cpp HOSTest dynamic_pressure example
         *  \section ex2 Expected output
         *  \snippet core/unit_tests/src/HOSTest.cpp HOSTest dynamic_pressure expected output
         *  */
        double dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                const double g,   //!< gravity (in m/s^2)
                                const double x,   //!< x-position in the NED frame (in meters)
                                const double y,   //!< y-position in the NED frame (in meters)
                                const double z,   //!< z-position in the NED frame (in meters)
                                const double eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
                                const double t    //!< Current time instant (in seconds)
                                ) const;

};
#endif /* HOS_HPP_ */
