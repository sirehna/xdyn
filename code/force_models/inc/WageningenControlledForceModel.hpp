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
#define NB_COEFF_KQ 47

#include "YamlPosition.hpp"

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
        struct Yaml
        {
            Yaml();
            std::string name;
            YamlPosition position_of_propeller_frame;
            double wake_coefficient;
            double relative_rotative_efficiency;
            double thrust_deduction_factor;
            bool rotating_clockwise;
            size_t number_of_blades;
            double blade_area_ratio;
            double diameter;
        };

        WageningenControlledForceModel(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;
        double Kt(const size_t Z, const double AE_A0, const double P_D, const double J) const;
        double Kq(const size_t Z, const double AE_A0, const double P_D, const double J) const;
        double advance_ratio(const BodyStates& states, std::map<std::string,double>& commands) const;
        static const std::string model_name;
        static Yaml parse(const std::string& yaml);

    private:
        WageningenControlledForceModel();
        void check(const double P_D, const double J) const;
        double w;
        double eta_R;
        double t;
        double kappa;
        size_t Z;
        double AE_A0;
        double D;
        double D4;
        double D5;

        const double ct[NB_COEFF_KT]; //!< Interpolation coefficient for Kt for the Wageningen B-series
        const size_t st[NB_COEFF_KT]; //!< Exponents for the advance ratio for Kt for the Wageningen B-series
        const size_t tt[NB_COEFF_KT]; //!< Exponents for P/D for Kt for the Wageningen B-series
        const size_t ut[NB_COEFF_KT]; //!< Exponents for the blade area ratio for Kt for the Wageningen B-series
        const size_t vt[NB_COEFF_KT]; //!< Exponents for number of blades for Kt for the Wageningen B-series

        const double cq[NB_COEFF_KQ]; //!< Interpolation coefficient for Kq for the Wageningen B-series
        const size_t sq[NB_COEFF_KQ]; //!< Exponents for the advance ratio for Kq for the Wageningen B-series
        const size_t tq[NB_COEFF_KQ]; //!< Exponents for P/D for Kq for the Wageningen B-series
        const size_t uq[NB_COEFF_KQ]; //!< Exponents for the blade area ratio for Kq for the Wageningen B-series
        const size_t vq[NB_COEFF_KQ]; //!< Exponents for number of blades for Kq for the Wageningen B-series
};

#endif /* WAGENINGENCONTROLLEDFORCEMODEL_HPP_ */
