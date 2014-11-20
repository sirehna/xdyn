/*
 * ForceParser.hpp
 *
 *  Created on: Oct 23, 2014
 *      Author: cady
 */

#ifndef CONTROLLEDFORCEBUILDER_HPP_
#define CONTROLLEDFORCEBUILDER_HPP_

#include <boost/optional/optional.hpp>

#include <ssc/macros.hpp>
#include TR1INC(memory)

#include <ssc/kinematics.hpp>
#include "ControllableForceModel.hpp"

struct EnvironmentAndFrames;

class ControlledForceBuilderInterface
{
    public:
        ControlledForceBuilderInterface() {}
        virtual ~ControlledForceBuilderInterface(){}
        virtual boost::optional<ControlledForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const = 0;
};

template <typename T>
class ControlledForceBuilder : public ControlledForceBuilderInterface
{
    public:
        ControlledForceBuilder() : ControlledForceBuilderInterface(){}
        boost::optional<ControlledForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};

typedef TR1(shared_ptr)<ControlledForceBuilderInterface> ControlledForceBuilderPtr;


#endif /* CONTROLLEDFORCEBUILDER_HPP_ */
