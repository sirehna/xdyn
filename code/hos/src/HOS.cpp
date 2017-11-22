/*
 * HOS.cpp
 *
 *  Created on: Nov 22, 2017
 *      Author: cady
 */



#include "HOS.hpp"


double HOS::dynamic_pressure(const double , //!< water density (in kg/m^3)
                                const double ,   //!< gravity (in m/s^2)
                                const double ,   //!< x-position in the NED frame (in meters)
                                const double ,   //!< y-position in the NED frame (in meters)
                                const double ,   //!< z-position in the NED frame (in meters)
                                const double , //!< Wave elevation at (x,y) in the NED frame (in meters)
                                const double     //!< Current time instant (in seconds)
                                ) const
{
    return 0;
}

ssc::kinematics::Point HOS::orbital_velocity(const double ,   //!< gravity (in m/s^2)
                                                const double ,   //!< x-position in the NED frame (in meters)
                                                const double ,   //!< y-position in the NED frame (in meters)
                                                const double ,   //!< z-position in the NED frame (in meters)
                                                const double ,   //!< z-position in the NED frame (in meters)
                                                const double   //!< Wave elevation at (x,y) in the NED frame (in meters)
                                               ) const
{
    return ssc::kinematics::Point("NED", 0, 0, 0);
}

double HOS::evaluate_rao(const double , //!< x-position of the RAO's calculation point in the NED frame (in meters)
                            const double , //!< y-position of the RAO's calculation point in the NED frame (in meters)
                            const double , //!< Current time instant (in seconds)
                            const std::vector<std::vector<double> >& , //!< Module of the RAO
                            const std::vector<std::vector<double> >&  //!< Phase of the RAO
                             ) const
{
    return 0;
}

double HOS::wave_height(const double , //!< x-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double , //!< y-coordinate of the point, relative to the centre of the NED frame, projected in the NED frame
                           const double   //!< Current instant (in seconds)
                           ) const
{
    return 0;
}
