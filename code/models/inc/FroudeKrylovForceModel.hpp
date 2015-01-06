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
        FroudeKrylovForceModel(const EnvironmentAndFrames& env);
        DF dF(const FacetIterator& that_facet,
              const EnvironmentAndFrames& env,
              const Body& body,
              const double t
              ) const;
        static const std::string model_name;

    private:
        FroudeKrylovForceModel();
        double pe(const Body& body, const std::vector<double>& x, const EnvironmentAndFrames& env) const;
};

#endif /* FROUDEKRYLOVFORCEMODEL_HPP_ */
