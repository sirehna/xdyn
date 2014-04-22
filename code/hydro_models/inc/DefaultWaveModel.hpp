/*
 * DefaultWaveModel.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef DEFAULTWAVEMODEL_HPP_
#define DEFAULTWAVEMODEL_HPP_

class DefaultWaveModel
{
    public:
        DefaultWaveModel(const double wave_height);

    private:
        DefaultWaveModel(); // Disabled
        double wave_height;
};

#endif /* DEFAULTWAVEMODEL_HPP_ */
