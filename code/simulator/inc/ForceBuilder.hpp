/*
 * ForceParser.hpp
 *
 *  Created on: Jun 16, 2014
 *      Author: cady
 */

#ifndef FORCEBUILDER_HPP_
#define FORCEBUILDER_HPP_

#include <boost/optional/optional.hpp>

#include "tr1_macros.hpp"
#include TR1INC(memory)

#include "EnvironmentAndFrames.hpp"
#include "ForceModel.hpp"

class ForceBuilderInterface
{
    public:
        ForceBuilderInterface() {}
        virtual ~ForceBuilderInterface(){}
        virtual boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const = 0;
};

template <typename T>
class ForceBuilder : public ForceBuilderInterface
{
    public:
        ForceBuilder() : ForceBuilderInterface(){}
        boost::optional<ForcePtr> try_to_parse(const std::string& model, const std::string& yaml, const EnvironmentAndFrames& env) const;
};

typedef TR1(shared_ptr)<ForceBuilderInterface> ForceBuilderPtr;


#endif /* FORCEBUILDER_HPP_ */
