/*
 * ExactHydrostaticForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#ifndef EXACTHYDROSTATICFORCEMODEL_HPP_
#define EXACTHYDROSTATICFORCEMODEL_HPP_

#include "FastHydrostaticForceModel.hpp"

/** \brief Same as FastHydrostaticForceModel but with the exact application point
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/ExactHydrostaticForceModelTest.cpp ExactHydrostaticForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/ExactHydrostaticForceModelTest.cpp ExactHydrostaticForceModelTest expected output
 */
class ExactHydrostaticForceModel : public FastHydrostaticForceModel
{
    public:
        ExactHydrostaticForceModel(const std::string& body_name, const EnvironmentAndFrames& env);
        static std::string model_name();

    private:
        ExactHydrostaticForceModel();
        EPoint get_application_point(const FacetIterator& that_facet, const BodyStates& states, const double zG) const;
};

#endif /* EXACTHYDROSTATICFORCEMODEL_HPP_ */
