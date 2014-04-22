/*
 * WaveModelInterface.hpp
 *
 *  Created on: 22 avr. 2014
 *      Author: cady
 */

#ifndef WAVEMODELINTERFACE_HPP_
#define WAVEMODELINTERFACE_HPP_

class Point;

class WaveModelInterface
{
    public:
        virtual ~WaveModelInterface() {}
        virtual double get_relative_wave_height(const Point& P) const = 0;
};

#endif /* WAVEMODELINTERFACE_HPP_ */
