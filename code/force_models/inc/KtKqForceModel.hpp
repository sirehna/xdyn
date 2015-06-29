/*
 * KtKqForceModel.hpp
 *
 *  Created on: Jun 27, 2015
 *      Author: cady
 */

#ifndef KTKQFORCEMODEL_HPP_
#define KTKQFORCEMODEL_HPP_

#include "AbstractWageningen.hpp"

/** \details This class was created to
 *  \addtogroup module
 *  \ingroup module
 *  \section ex1 Example
 *  \snippet module/unit_tests/src/KtKqForceModelTest.cpp KtKqForceModelTest example
 *  \section ex2 Expected output
 *  \snippet module/unit_tests/src/KtKqForceModelTest.cpp KtKqForceModelTest expected output
 */
class KtKqForceModel : public AbstractWageningen
{
    public:
        struct Yaml : public AbstractWageningen::Yaml
        {
            Yaml();
            Yaml(const AbstractWageningen::Yaml& y);
            std::vector<double> J;
            std::vector<double> Kt;
            std::vector<double> Kq;
        };

        static Yaml parse(const std::string& yaml);
    private:
};
#endif /* KTKQFORCEMODEL_HPP_ */
