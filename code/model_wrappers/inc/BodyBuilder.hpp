/*
 * BodyBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef BODYBUILDER_HPP_
#define BODYBUILDER_HPP_

#include "Body.hpp"
#include "YamlRotation.hpp"

class YamlInertiaMatrix;
class YamlAngle;

/** \author cec
 *  \date Jun 17, 2014, 12:39:59 PM
 *  \brief Builds a Body object from the YAML & STL describing it
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/BodyBuilderTest.cpp BodyBuilderTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/BodyBuilderTest.cpp BodyBuilderTest expected output
 */
class BodyBuilder
{
    public:
        /**  \details It makes no sense to build a 'Body' object without knowing
         *            the rotation conventions, which is why this is the only
         *            constructor available.
          */
        BodyBuilder(const YamlRotation& convention);

        /**  \brief Build a 'Body' object from YAML & STL data
          *  \returns New Body object
          */
        Body build(const YamlBody& input, const VectorOfVectorOfPoints& mesh) const;

        /**  \details Only used for testing purposes when we don't want to go
         *            through the hassle of defining the inertia matrix & initial
         *            positions
          *  \returns New Body object
          */
        Body build(const std::string& name, const VectorOfVectorOfPoints& mesh) const;

        /**  \details Converts (phi,theta,psi) to a rotation matrix, taking the
         *            rotation convention into account
          */
        RotationMatrix angle2matrix(const YamlAngle& a) const;

    private:
        BodyBuilder(); //Disabled

        /**  \brief Utility function used by angle2matrix
         *   \returns True if 'convention' == [first,second,third]
          */
        bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third) const;

        void add_inertia(Body& body, const YamlInertiaMatrix& rigid_body_inertia, const YamlInertiaMatrix& added_mass) const;

        /**  \details Converts the external YAML data structure (several std::vectors)
         *            to an Eigen::Matrix used for calculations
          */
        Eigen::Matrix<double,6,6> convert(const YamlInertiaMatrix& M) const;

        /**  \brief Puts the mesh in the body frame
          *  \details Uses the body frame's initial position relative to the mesh
          */
        void change_mesh_ref_frame(Body& body, const VectorOfVectorOfPoints& mesh) const;

        YamlRotation rotations; //!< Rotation convention (describes how we can build a rotation matrix from three angles)
};

#endif /* BODYBUILDER_HPP_ */
