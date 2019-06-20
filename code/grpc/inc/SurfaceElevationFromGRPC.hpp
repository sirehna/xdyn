/*
 * GRPCWaveModel.hpp
 *
 *  Created on: May 27, 2019
 *      Author: cady
 */

#ifndef ENVIRONMENT_MODELS_INC_GRPCWAVEMODEL_HPP_
#define ENVIRONMENT_MODELS_INC_GRPCWAVEMODEL_HPP_

#include "SurfaceElevationInterface.hpp"

struct YamlGRPC;

/** \brief Call an external "wave" model through a gRPC interface (cf. https://grpc.io/)
 *  \ingroup wave_models
 */
class SurfaceElevationFromGRPC : public SurfaceElevationInterface
{
    public:
        SurfaceElevationFromGRPC(const YamlGRPC& yaml, const ssc::kinematics::PointMatrixPtr& output_mesh);


        std::vector<std::vector<double> > get_wave_directions_for_each_model() const;
        std::vector<std::vector<double> > get_wave_angular_frequency_for_each_model() const;
        /**  \brief Calculate radiation forces using first order force RAO
          *  \returns Force (or torque), depending on the RAO
          */
        double evaluate_rao(const double x, //!< x-position of the RAO's calculation point in the NED frame (in meters)
                            const double y, //!< y-position of the RAO's calculation point in the NED frame (in meters)
                            const double t, //!< Current time instant (in seconds)
                            const std::vector<std::vector<double> >& rao_module, //!< Module of the RAO
                            const std::vector<std::vector<double> >& rao_phase //!< Phase of the RAO
                             ) const;


        /**  \brief Surface elevation
          *  \returns Elevation of a point at a given instant, in meters.
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 76
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 29
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation IV.20, page 125
          */
        std::vector<double> wave_height(const std::vector<double>& x,                                  //!< x-position in the NED frame (in meters)
                                      const std::vector<double>& y,                                  //!< y-position in the NED frame (in meters)
                                      const double t                                   //!< Current time instant (in seconds)
                                      ) const;

        /**  \brief Unsteady pressure field induced by undisturbed waves. Used to compute the Froude-Krylov forces.
          *  \details Also called "subsurface pressure" (by DNV), "unsteady pressure" (by Faltinsen) or constant pressure contour (by Lloyd)
          *           The dynamic pressure is in fact one of the terms of Bernoulli's equation, which can be derived from the conservation
          *           of energy for a fluid in motion.
          *           \f[\int_C \frac{\partial \Phi_I(x,y,z,t)}{\partial t}\f]
          *  \returns Dynamic pressure in Pascal
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 185
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
          *  \see "Hydrodynamique navale : théorie et modèles", 2009, Alain Bovis, Les Presses de l'ENSTA, equation VI.34, page 183
          *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 68
          *  \see "The dynamic of marine craft", 2004, Lewandoski, page 148
          *  \snippet environment_models/unit_tests/src/AiryTest.cpp AiryTest elevation_example
          */
        std::vector<double> dynamic_pressure(const double rho, //!< water density (in kg/m^3)
                                             const double g,   //!< gravity (in m/s^2)
                                             const std::vector<double>& x,   //!< x-position in the NED frame (in meters)
                                             const std::vector<double>& y,   //!< y-position in the NED frame (in meters)
                                             const std::vector<double>& z,   //!< z-position in the NED frame (in meters)
                                             const std::vector<double>& eta, //!< Wave elevation at (x,y) in the NED frame (in meters)
                                             const double t    //!< Current time instant (in seconds)
                                            ) const;

        /**  \brief Wave velocity (projected in the NED frame, at a point (x,y,z)).
          *  \returns Orbital velocity in m/s
          *  \see "Environmental Conditions and Environmental Loads", April 2014, DNV-RP-C205, Det Norske Veritas AS, page 47
          *  \see "Hydrodynamique des Structures Offshore", 2002, Bernard Molin, Editions TECHNIP, page 70
          *  \see "Sea Loads on Ships and Offshore Structures", 1990, O.M. Faltinsen, Cambridge Ocean Technology Series, page 16
          *  \see "Seakeeping: ship behaviour in rough weather", 1989, A. R. J. M. Lloyd, Ellis Horwood Series in Marine Technology, page 75
          *  \see "The dynamic of marine craft", 2004, Lewandoski, page 148
          */
        /**  \author cec
          *  \date Feb 3, 2015, 10:06:45 AM
          *  \brief Orbital velocity
          *  \returns Velocity of the fluid at a given point & instant, in m/s
          */
        virtual ssc::kinematics::Point orbital_velocity(const double g,   //!< gravity (in m/s^2)
                                                        const double x,   //!< x-position in the NED frame (in meters)
                                                        const double y,   //!< y-position in the NED frame (in meters)
                                                        const double z,   //!< z-position in the NED frame (in meters)
                                                        const double t,   //!< z-position in the NED frame (in meters)
                                                        const double eta  //!< Wave elevation at (x,y) in the NED frame (in meters)
                                                       ) const;
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
                                                             ) const;

    private:
        SurfaceElevationFromGRPC(); // Disabled (private & without implementation)
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;
};


#endif /* ENVIRONMENT_MODELS_INC_GRPCWAVEMODEL_HPP_ */
