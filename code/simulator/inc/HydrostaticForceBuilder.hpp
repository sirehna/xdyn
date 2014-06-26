/*
 * HydrostaticForceBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef HYDROSTATICFORCEBUILDER_HPP_
#define HYDROSTATICFORCEBUILDER_HPP_

#include "ForceBuilder.hpp"
#include "HydrostaticForceModel.hpp"

template <>
class ForceBuilder<HydrostaticForceModel> : public ForceBuilderInterface
{
    public:
        ForceBuilder();
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};


#endif /* HYDROSTATICFORCEBUILDER_HPP_ */
