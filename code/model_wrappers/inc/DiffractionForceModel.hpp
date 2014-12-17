/*
 * DiffractionForceModel.hpp
 *
 */

#ifndef DIFFRACTIONFORCEMODEL_HPP_
#define DIFFRACTIONFORCEMODEL_HPP_

#include <ssc/macros.hpp>

#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"

struct YamlDiffraction;

/** \brief Diffraction forces
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/DiffractionForceModelTest.cpp DiffractionForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/DiffractionForceModelTest.cpp DiffractionForceModelTest expected output
 */
class DiffractionForceModel : public ForceModel
{
    public:
        DiffractionForceModel(const YamlDiffraction& data, const EnvironmentAndFrames& end);
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;

    private:
        DiffractionForceModel();
        class Impl;
        TR1(shared_ptr)<Impl> pimpl;

};

#endif /* DIFFRACTIONFORCEMODEL_HPP_ */
