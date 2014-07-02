/*
 * WaveParser.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef WAVEBUILDER_HPP_
#define WAVEBUILDER_HPP_

#include <boost/optional/optional.hpp>

class WaveModelInterface;

class WaveBuilderInterface
{
    public:
        WaveBuilderInterface() {}
        virtual ~WaveBuilderInterface(){}
        virtual boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const = 0;
};

template <typename T>
class WaveBuilder : public WaveBuilderInterface
{
    public:
        WaveBuilder() : WaveBuilderInterface(){}
        boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};


typedef TR1(shared_ptr)<WaveBuilderInterface> WaveBuilderPtr;
typedef TR1(shared_ptr)<WaveModelInterface> WavePtr;


#endif /* WAVEBUILDER_HPP_ */
