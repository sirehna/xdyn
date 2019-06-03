/*
 * AbstractWageningen.hpp
 *
 *  Created on: Jun 28, 2015
 *      Author: cady
 */

#ifndef ABSTRACTWAGENINGEN_HPP_
#define ABSTRACTWAGENINGEN_HPP_

#include "ControllableForceModel.hpp"

/** \details This class was created to
 *  \addtogroup module
 *  \ingroup module
 *  \section ex1 Example
 *  \snippet module/unit_tests/src/AbstractWageningenTest.cpp AbstractWageningenTest example
 *  \section ex2 Expected output
 *  \snippet module/unit_tests/src/AbstractWageningenTest.cpp AbstractWageningenTest expected output
 */
class AbstractWageningen : public ControllableForceModel
{
    public:
        struct Yaml
        {
            Yaml();
            virtual ~Yaml(){}
            std::string name;
            YamlPosition position_of_propeller_frame;
            double wake_coefficient;
            double relative_rotative_efficiency;
            double thrust_deduction_factor;
            bool rotating_clockwise;
            double diameter;
        };
        AbstractWageningen(const Yaml& input, const std::string& body_name, const EnvironmentAndFrames& env);
        virtual double get_Kt(const std::map<std::string,double>& commands, const double J) const = 0;
        virtual double get_Kq(const std::map<std::string,double>& commands, const double J) const = 0;
        ssc::kinematics::Vector6d get_force(const BodyStates& states, const double t, std::map<std::string,double> commands) const;
        double advance_ratio(const BodyStates& states, std::map<std::string,double>& commands) const;
        static Yaml parse(const std::string& yaml);

    private:
        AbstractWageningen();
        double w;
        double eta_R;
        double t;
        double kappa;
        double D;
        double D4;
        double D5;
};
#endif /* ABSTRACTWAGENINGEN_HPP_ */
