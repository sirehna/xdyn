/*
 * WageningenControlledForceModel.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef WAGENINGENCONTROLLEDFORCEMODEL_HPP_
#define WAGENINGENCONTROLLEDFORCEMODEL_HPP_

#include "ControllableForceModel.hpp"

class YamlWageningen;

/** \brief
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/WageningenControlledForceModelTest.cpp WageningenControlledForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/WageningenControlledForceModelTest.cpp WageningenControlledForceModelTest expected output
 */
class WageningenControlledForceModel : public ControllableForceModel
{
    public:
        WageningenControlledForceModel(const YamlWageningen& input);
        ssc::kinematics::Wrench get_force(const Body& body, const double t, std::map<std::string,double> commands) const;

    private:
        WageningenControlledForceModel();
        double w;
        double eta_R;
        double t;
        double kappa;
        size_t Z;
        double AE_A0;
};

#endif /* WAGENINGENCONTROLLEDFORCEMODEL_HPP_ */
