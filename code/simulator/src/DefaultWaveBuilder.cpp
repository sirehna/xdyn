/*
 * DefaultWaveBuilder.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cady
 */

#include "DefaultWaveBuilder.hpp"
#include "WaveBuilder.hpp"
#include "DefaultWaveModel.hpp"
#include "environment_parsers.hpp"
#include "SimulatorBuilderException.hpp"

TR1(shared_ptr)<WaveModelInterface> build_default_wave_model(const std::string& yaml);
TR1(shared_ptr)<WaveModelInterface> build_default_wave_model(const std::string& yaml)
{
    if (yaml.empty())
    {
        THROW(__PRETTY_FUNCTION__, SimulatorBuilderException, "No yaml data detected for default wave model (expected 'constant wave height in NED frame: {value: xx, unit: yy})'");
    }
    return TR1(shared_ptr)<WaveModelInterface>(new DefaultWaveModel(parse_default_wave_model(yaml)));
}


boost::optional<TR1(shared_ptr)<WaveModelInterface> > WaveBuilder<DefaultWaveModel>::try_to_parse(const std::string& model, const std::string& yaml) const
{
    boost::optional<TR1(shared_ptr)<WaveModelInterface> > ret;
    if (model == "no waves") ret.reset(build_default_wave_model(yaml));
    return ret;
}
