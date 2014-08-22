/*
 * GravityForceBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef GRAVITYFORCEBUILDER_HPP_
#define GRAVITYFORCEBUILDER_HPP_

#include "ForceBuilder.hpp"
#include "GravityForceModel.hpp"

template <>
class ForceBuilder<GravityForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};


#endif /* GRAVITYFORCEBUILDER_HPP_ */
