/*
 * WaveModelBuilder.hpp
 *
 *  Created on: Aug 12, 2014
 *      Author: cady
 */

#ifndef WAVEMODELBUILDER_HPP_
#define WAVEMODELBUILDER_HPP_

#include <string>

#include <boost/optional/optional.hpp>

#include "tr1_macros.hpp"
#include TR1INC(memory)

class WaveModel;

/** \brief Interface to all WaveModelBuilder<T>. Used to build, eg. Airy
 *  \details Allows us to store WaveModelBuilders with different template
 *  parameters inside a same collection (eg. vector<WaveModelBuilderPtr>)
 *  \addtogroup simulator
 *  \ingroup simulator
 */
class WaveModelBuilderInterface
{
    public:
        WaveModelBuilderInterface();
        virtual ~WaveModelBuilderInterface();
        virtual boost::optional<TR1(shared_ptr)<WaveModel> > try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const = 0;

};

template <typename T> class WaveModelBuilder
{
    public:
        WaveModelBuilder() {}
        ~WaveModelBuilder() {}
        boost::optional<TR1(shared_ptr)<WaveModel> > try_to_parse(const std::string& model, const DiscreteDirectionalWaveSpectrum& spectrum, const std::string& yaml) const = 0;
};

typedef TR1(shared_ptr)<WaveModelBuilderInterface> WaveModelBuilderPtr;

#endif /* WAVEMODELBUILDER_HPP_ */
