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


class ZGCalculator
{
    public:
        ZGCalculator() : tz(0), r31(0), r32(0), r33(0) {}
        void update_transform(const ssc::kinematics::Transform& T)
        {
            tz = T.get_point().z();
            const auto R = T.get_rot();
            r31 = R(2,0);
            r32 = R(2,1);
            r33 = R(2,2);
        }
        double get_zG_in_NED(const Eigen::Vector3d& G_in_body) const
        {
            return tz + r31*G_in_body(0) + r32*G_in_body(1) + r33*G_in_body(2);
        }

    private:
        double tz, r31, r32, r33;
};


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

        SurfaceForceModel(const std::string& name, const std::string& body_name_, const EnvironmentAndFrames& env);
        virtual ~SurfaceForceModel();
        ssc::kinematics::Wrench operator()(const BodyStates& states, const double t) const;
        virtual std::function<DF(const FacetIterator &,
                                 const size_t,
                                 const EnvironmentAndFrames &,
                                 const BodyStates &,
                                 const double
                                )> get_dF(const FacetIterator &begin_facet,
                                          const FacetIterator &end_facet,
                                          const EnvironmentAndFrames &env,
                                          const BodyStates &states,
                                          const double t
                                         ) const = 0;

    /**  \brief Compute potential energy of the hydrostatic force model
      */
        double potential_energy(const BodyStates& states, const std::vector<double>& x) const;

        bool is_a_surface_force_model() const;

    private:
        SurfaceForceModel();
        virtual FacetIterator begin(const MeshIntersectorPtr& intersector) const = 0;
        virtual FacetIterator end(const MeshIntersectorPtr& intersector) const = 0;
        virtual double pe(const BodyStates& states, const std::vector<double>& x, const EnvironmentAndFrames& env) const = 0;

    protected:
        EnvironmentAndFrames env;

    private:
        ssc::kinematics::Point g_in_NED;

    protected:
        TR1(shared_ptr)<ZGCalculator> zg_calculator;
};

#endif /* SURFACEFORCEMODEL_HPP_ */
