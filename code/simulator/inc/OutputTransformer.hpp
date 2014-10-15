/*
 * OutputGenerator.hpp
 *
 *  Created on: Jun 30, 2014
 *      Author: cady
 */

#ifndef OUTPUTTRANSFORMER_HPP_
#define OUTPUTTRANSFORMER_HPP_

#include <map>
#include <string>

#include <ssc/kinematics.hpp>
#include <ssc/macros.hpp>
#include TR1INC(memory)

#include "EnvironmentAndFrames.hpp"
#include "Body.hpp"
#include "Res.hpp"
#include "StateMacros.hpp"
#include "YamlSimulatorInput.hpp"

class SimulatorBuilder;

/** \author cec
 *  \date Jul 1, 2014, 9:52:49 AM
 *  \brief Post-processes the simulation results to compute derived quantities.
 *  \details For example, can calculate the position of any point, relative to
 *  any frame & projected in any frame. Works for angles, velocities & accelerations.
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/OutputTransformerTest.cpp OutputTransformerTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/OutputTransformerTest.cpp OutputTransformerTest expected output
 */
class OutputTransformer
{
    public:
        OutputTransformer(const SimulatorBuilder& input);
        std::map<std::string,double> operator()(const Res& r) const;

    private:
        void update_kinematics(const StateType& x) const;
        void fill(std::map<std::string,double>& out, const YamlPositionOutput& position) const;
        void fill(std::map<std::string,double>& out, const YamlAnglesOutput& angle) const;
        void fill_energy(std::map<std::string,double>& out, const size_t i, const StateType& x) const;
        ssc::kinematics::EulerAngles convert(const ssc::kinematics::RotationMatrix& R) const;
        double compute_kinetic_energy(const size_t i, const StateType& x) const;
        double compute_potential_energy(const size_t i, const StateType& x) const;

        YamlSimulatorInput input;
        std::vector<Body> bodies;
        std::map<std::string,ssc::kinematics::Point> points;
        TR1(shared_ptr)<ssc::kinematics::Kinematics> k;
        std::vector<ListOfForces> forces;
        EnvironmentAndFrames env;
};

#endif /* OUTPUTTRANSFORMER_HPP_ */
