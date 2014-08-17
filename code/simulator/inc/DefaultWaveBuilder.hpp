/*
 * DefaultWaveBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef DEFAULTWAVEBUILDER_HPP_
#define DEFAULTWAVEBUILDER_HPP_

#include "DefaultSurfaceElevation.hpp"
#include "WaveBuilder.hpp"

template <>
class WaveBuilder<DefaultSurfaceElevation> : public WaveBuilderInterface
{
    public:
        WaveBuilder(const TR1(shared_ptr)<std::vector<DirectionalSpreadingBuilderPtr> >& directional_spreading_parsers_,
                    const TR1(shared_ptr)<std::vector<SpectrumBuilderPtr> >& spectrum_parsers_);
        boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};



#endif /* DEFAULTWAVEBUILDER_HPP_ */
