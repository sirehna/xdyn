/*
 * ResistanceCurveForceModel.hpp
 *
 *  Created on: Oct 24, 2014
 *      Author: cady
 */

#ifndef RESISTANCECURVEFORCEMODEL_HPP_
#define RESISTANCECURVEFORCEMODEL_HPP_

#include <ssc/macros.hpp>

#include "ForceModel.hpp"

class EnvironmentAndFrames;

/** \brief Resistance curve given by interpolation table
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/ResistanceCurveForceModelTest.cpp ResistanceCurveForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/ResistanceCurveForceModelTest.cpp ResistanceCurveForceModelTest expected output
 */
class ResistanceCurveForceModel : public ForceModel
{
    public:
        struct Yaml
        {
            Yaml();
            std::vector<double> Va;
            std::vector<double> R;
        };
        ResistanceCurveForceModel(const Yaml& data, const EnvironmentAndFrames& env);
        static Yaml parse(const std::string& yaml);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;
        static const std::string model_name;

    private:
        ResistanceCurveForceModel();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* RESISTANCECURVEFORCEMODEL_HPP_ */
