/*
 * WaveParser.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef WAVEBUILDER_HPP_
#define WAVEBUILDER_HPP_

#include <vector>

#include <boost/optional/optional.hpp>
#include "tr1_macros.hpp"
#include TR1INC(memory)

#include "DirectionalSpreadingBuilder.hpp"
#include "SpectrumBuilder.hpp"


class WaveModelInterface;

class WaveBuilderInterface
{
    public:
        WaveBuilderInterface(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                             const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_) :
                             directional_spreading_parsers(directional_spreading_parsers_),
                             spectrum_parsers(spectrum_parsers_)
        {}
        virtual ~WaveBuilderInterface(){}
        virtual boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const = 0;

    private:
        WaveBuilderInterface();
        TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> > directional_spreading_parsers;
        TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> > spectrum_parsers;
};

template <typename T>
class WaveBuilder : public WaveBuilderInterface
{
    public:
        WaveBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_) : WaveBuilderInterface(directional_spreading_parsers_,spectrum_parsers_)
        {}
        boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;

    private:
        WaveBuilder(); // Disabled
};


typedef TR1(shared_ptr)<WaveBuilderInterface> WaveBuilderPtr;
typedef TR1(shared_ptr)<WaveModelInterface> WavePtr;


#endif /* WAVEBUILDER_HPP_ */
