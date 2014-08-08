/*
 * WaveModelFromSpectraBuilder.hpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#ifndef WAVEMODELFROMSPECTRABUILDER_HPP_
#define WAVEMODELFROMSPECTRABUILDER_HPP_

#include "Airy.hpp"
#include "DefaultWaveModel.hpp"
#include "WaveBuilder.hpp"

template <>
class WaveBuilder<Airy> : public WaveBuilderInterface
{
    public:
        WaveBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;

    private:
        WaveBuilder();
};

#endif /* WAVEMODELFROMSPECTRABUILDER_HPP_ */
