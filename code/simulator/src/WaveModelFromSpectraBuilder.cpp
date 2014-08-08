/*
 * WaveModelFromSpectraBuilder.cpp
 *
 *  Created on: Aug 7, 2014
 *      Author: cady
 */

#include "WaveModelFromSpectraBuilder.hpp"
#include "YamlWaveModelInput.hpp"

boost::optional<TR1(shared_ptr)<WaveModelInterface> > WaveBuilder<Airy>::try_to_parse(const std::string& model, const std::string& ) const
{
    boost::optional<TR1(shared_ptr)<WaveModelInterface> > ret;
    if (model == "waves") // The "model" key is always "wave", except for the default wave model "no waves"
    {
    }
    return ret;
}

