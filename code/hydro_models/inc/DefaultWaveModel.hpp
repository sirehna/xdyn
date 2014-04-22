/*
 * DefaultWaveModel.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef DEFAULTWAVEMODEL_HPP_
#define DEFAULTWAVEMODEL_HPP_

#include "WaveModelInterface.hpp"

class DefaultWaveModel : public WaveModelInterface
{
    public:
        DefaultWaveModel(const double wave_height);
        double get_relative_wave_height(const Point& P) const;

    private:
        DefaultWaveModel(); // Disabled
        double wave_height;
};

#endif /* DEFAULTWAVEMODEL_HPP_ */
