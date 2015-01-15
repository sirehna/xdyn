/*
 * SimpleTrackKeepingController.hpp
 *
 *  Created on: Jan 15, 2015
 *      Author: cady
 */

#ifndef SIMPLETRACKKEEPINGCONTROLLER_HPP_
#define SIMPLETRACKKEEPINGCONTROLLER_HPP_

#include <string>

class SimpleTrackKeepingController
{
    public:
        struct Yaml
        {
            Yaml();
            std::string name;
            double ksi;
            double Tp;
        };
        static Yaml parse(const std::string& yaml);
};

#endif /* SIMPLETRACKKEEPINGCONTROLLER_HPP_ */
