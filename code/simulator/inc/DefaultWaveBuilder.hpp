/*
 * DefaultWaveBuilder.hpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#ifndef DEFAULTWAVEBUILDER_HPP_
#define DEFAULTWAVEBUILDER_HPP_

#include "DefaultWaveModel.hpp"
#include "WaveBuilder.hpp"

template <>
class WaveBuilder<DefaultWaveModel> : public WaveBuilderInterface
{
    public:
        WaveBuilder();
        boost::optional<TR1(shared_ptr)<WaveModelInterface> > try_to_parse(const std::string& model, const std::string& yaml) const;
};



#endif /* DEFAULTWAVEBUILDER_HPP_ */
