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
 *  \brief
 *  \details
 *  \ingroup simulator
 *  \section ex1 Example
 *  \snippet simulator/unit_tests/src/BodyBuilderTest.cpp BodyBuilderTest example
 *  \section ex2 Expected output
 *  \snippet simulator/unit_tests/src/BodyBuilderTest.cpp BodyBuilderTest expected output
 */
class BodyBuilder
{
    public:
        BodyBuilder(const YamlRotation& convention);
        Body build(const YamlBody& input, const VectorOfVectorOfPoints& mesh) const;

    private:
        BodyBuilder();
        RotationMatrix angle2matrix(const YamlAngle& a) const;
        bool match(const std::vector<std::string>& convention, const std::string& first, const std::string& second, const std::string& third) const;

        YamlRotation rotations;
};

#endif /* BODYBUILDER_HPP_ */
