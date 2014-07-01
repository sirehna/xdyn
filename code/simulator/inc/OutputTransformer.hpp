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

#include "Body.hpp"
#include "Point.hpp"
#include "Res.hpp"
#include "StateMacros.hpp"
#include "YamlSimulatorInput.hpp"

#include "tr1_macros.hpp"
#include TR1INC(memory)

class SimulatorBuilder;
class Kinematics;

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

        YamlSimulatorInput input;
        std::vector<Body> bodies;
        std::map<std::string,Point> points;
        TR1(shared_ptr)<Kinematics> k;
};

#endif /* OUTPUTTRANSFORMER_HPP_ */
