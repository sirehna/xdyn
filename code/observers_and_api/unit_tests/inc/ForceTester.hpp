/*
 * ForceTester.hpp
 *
 *  Created on: Feb 16, 2015
 *      Author: cady
 */


#ifndef FORCETESTER_HPP_
#define FORCETESTER_HPP_

#include "Sim.hpp"

std::vector<double> get_states(const double x, const double y,
        const double z, const double phi, const double theta, const double psi);

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
        double immersed_volume(const double x,
                               const double y,
                               const double z,
                               const double phi,
                               const double theta,
                               const double psi);
        double emerged_volume(const double x,
                              const double y,
                              const double z,
                              const double phi,
                              const double theta,
                              const double psi);
        double immersed_surface(const double x,
                               const double y,
                               const double z,
                               const double phi,
                               const double theta,
                               const double psi);
        double emerged_surface(const double x,
                              const double y,
                              const double z,
                              const double phi,
                              const double theta,
                              const double psi);
        std::vector<double> set_states(const double x, const double y,
                const double z, const double phi, const double theta,
                const double psi);

        template <typename ForceType> ForceTester& add()
        {
            forces.push_back(ForcePtr(new ForceType(body->get_name(), env)));
            return *this;
        }

        template <typename ForceType> ForceTester& add(const std::string& yaml)
        {
            const typename ForceType::Yaml params = ForceType::parse(yaml);
            forces.push_back(ForcePtr(new ForceType(params, body->get_name(), env)));
            return *this;
        }

        EPoint center_of_buoyancy_in_ned_frame(const double x,
                                               const double y,
                                               const double z,
                                               const double phi,
                                               const double theta,
                                               const double psi);

        boost::optional<double> gm(const double x,
                                   const double y,
                                   const double z,
                                   const double phi,
                                   const double theta,
                                   const double psi);

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
