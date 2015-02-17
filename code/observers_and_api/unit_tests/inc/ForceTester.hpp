/*
 * ForceTester.hpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#ifndef FORCETESTER_HPP_
#define FORCETESTER_HPP_

#include "Sim.hpp"

class ForceTester
{
    public:
        ForceTester(const std::string& yaml, const std::string& stl);
        ForceTester(const std::string& yaml, const VectorOfVectorOfPoints& stl);
        ssc::kinematics::Wrench force_in_ned(const double x,
                                             const double y,
                                             const double z,
                                             const double phi,
                                             const double theta,
                                             const double psi);

        template <typename ForceType> ForceTester& add()
        {
            forces.push_back(ForcePtr(new ForceType(body->get_name(), env)));
            return *this;
        }

    private:
        ForceTester();
        Sim make_sim(const std::string& yaml, const std::string& stl) const;
        Sim make_sim(const std::string& yaml, const VectorOfVectorOfPoints& stl) const;
        std::vector<ForcePtr> forces;
        BodyPtr body;
        EnvironmentAndFrames env;
        double current_instant;
};

#endif  /* FORCETESTER_HPP_ */
