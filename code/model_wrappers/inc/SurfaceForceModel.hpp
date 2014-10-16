/*
 * SurfaceForceModel.hpp
 *
 *  Created on: Oct 2, 2014
 *      Author: cady
 */

#ifndef SURFACEFORCEMODEL_HPP_
#define SURFACEFORCEMODEL_HPP_

#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"
#include "GeometricTypes3d.hpp"
#include "MeshIntersector.hpp"

/** \brief Models a force integrated of a surface mesh
 *  \details Implements the integration of the force in operator()
 *  \addtogroup model_wrappers
 *  \ingroup model_wrappers
 *  \section ex1 Example
 *  \snippet model_wrappers/unit_tests/src/SurfaceForceModelTest.cpp SurfaceForceModelTest example
 *  \section ex2 Expected output
 *  \snippet model_wrappers/unit_tests/src/SurfaceForceModelTest.cpp SurfaceForceModelTest expected output
 */
class SurfaceForceModel : public ForceModel
{
    public:
        struct DF
        {
            DF(const EPoint& dF_, const EPoint& C_) : dF(dF_), C(C_)
            {
            }

            EPoint dF; //!< Elementary force
            EPoint C; //!< Point of application (used to calculate the torque)
        };

        SurfaceForceModel(const EnvironmentAndFrames& env);
        virtual ~SurfaceForceModel();
        ssc::kinematics::Wrench operator()(const Body& body, const double t) const;
        virtual DF dF(const FacetIterator& that_facet,
                      const EnvironmentAndFrames& env,
                      const Body& body,
                      const double t
                      ) const = 0;

    /**  \brief Compute potential energy of the hydrostatic force model
      */
        double potential_energy(const Body& body, const std::vector<double>& x) const;


    private:
        SurfaceForceModel();
        virtual FacetIterator begin(const MeshIntersectorPtr& intersector) const = 0;
        virtual FacetIterator end(const MeshIntersectorPtr& intersector) const = 0;
        virtual double pe(const Body& body, const std::vector<double>& x, const EnvironmentAndFrames& env) const = 0;
        EnvironmentAndFrames env;
        ssc::kinematics::Point g_in_NED;
};

#endif /* SURFACEFORCEMODEL_HPP_ */
