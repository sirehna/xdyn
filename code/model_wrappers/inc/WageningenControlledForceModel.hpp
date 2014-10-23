/*
 * WageningenControlledForceModel.hpp
 *
 *  Created on: Oct 22, 2014
 *      Author: cady
 */

#ifndef WAGENINGENCONTROLLEDFORCEMODEL_HPP_
#define WAGENINGENCONTROLLEDFORCEMODEL_HPP_

#include "ControllableForceModel.hpp"

#define NB_COEFF_KT 39

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
        double Kt(const size_t Z, const double AE_A0, const double P_D, const double J) const;
        double Kq(const size_t Z, const double AE_A0, const double P_D, const double J) const;

    private:
        WageningenControlledForceModel();
        double w;
        double eta_R;
        double t;
        double kappa;
        size_t Z;
        double AE_A0;

        const double ct[NB_COEFF_KT]; //!< Interpolation coefficient for Kt for the Wageningen B-series
        const size_t st[NB_COEFF_KT]; //!< Exponents for the advance ratio for Kt for the Wageningen B-series
        const size_t tt[NB_COEFF_KT]; //!< Exponents for P/D for Kt for the Wageningen B-series
        const size_t ut[NB_COEFF_KT]; //!< Exponents for the blade area ratio for Kt for the Wageningen B-series
        const size_t vt[NB_COEFF_KT]; //!< Exponents for number of blades for Kt for the Wageningen B-series
};

#endif /* WAGENINGENCONTROLLEDFORCEMODEL_HPP_ */
