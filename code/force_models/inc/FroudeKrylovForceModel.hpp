/*
 * FroudeKrylovForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#ifndef FROUDEKRYLOVFORCEMODEL_HPP_
#define FROUDEKRYLOVFORCEMODEL_HPP_

#include "ImmersedSurfaceForceModel.hpp"

/** \brief
 *  \details
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/FroudeKrylovForceModelTest.cpp FroudeKrylovForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/FroudeKrylovForceModelTest.cpp FroudeKrylovForceModelTest expected output
 */
class FroudeKrylovForceModel : public ImmersedSurfaceForceModel
{
    public:
        FroudeKrylovForceModel(const std::string& body_name, const EnvironmentAndFrames& env);
        std::function<DF(const FacetIterator &,
                         const size_t,
                         const EnvironmentAndFrames &,
                         const BodyStates &,
                         const double
                        )> get_dF(const FacetIterator& begin_facet,
                                  const FacetIterator& end_facet,
                                  const EnvironmentAndFrames& env,
                                  const BodyStates& states,
                                  const double t
                                 ) const;
        static std::string model_name();

    private:
        FroudeKrylovForceModel();
        double pe(const BodyStates& states, const std::vector<double>& x, const EnvironmentAndFrames& env) const;
};

#endif /* FROUDEKRYLOVFORCEMODEL_HPP_ */
