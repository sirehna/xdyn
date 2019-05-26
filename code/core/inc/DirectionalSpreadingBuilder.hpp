/*
 * DirectionalSpreadingBuilder.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef DIRECTIONALSPREADINGBUILDER_HPP_
#define DIRECTIONALSPREADINGBUILDER_HPP_

#include <ssc/macros.hpp>
#include TR1INC(memory)
#include <boost/optional/optional.hpp>
#include <string>

class WaveDirectionalSpreading;

typedef TR1(shared_ptr)<WaveDirectionalSpreading> WaveDirectionalSpreadingPtr;

class DirectionalSpreadingBuilderInterface
{
    public:
        DirectionalSpreadingBuilderInterface() {}
        virtual ~DirectionalSpreadingBuilderInterface(){}
        virtual boost::optional<WaveDirectionalSpreadingPtr> try_to_parse(
            const std::string& model,
            const std::string& yaml
            ) const = 0;
};

template <typename T>
class DirectionalSpreadingBuilder : public DirectionalSpreadingBuilderInterface
{
    public:
        DirectionalSpreadingBuilder() : DirectionalSpreadingBuilderInterface(){}
        boost::optional<WaveDirectionalSpreadingPtr> try_to_parse(
            const std::string& model,
            const std::string& yaml
            ) const;
};

typedef TR1(shared_ptr)<DirectionalSpreadingBuilderInterface> DirectionalSpreadingBuilderPtr;

#endif /* DIRECTIONALSPREADINGBUILDER_HPP_ */
