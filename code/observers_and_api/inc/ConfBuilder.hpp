/*
 * ConfBuilder.hpp
 *
 *  Created on: 22 oct. 2015
 *      Author: sirehna
 */

#ifndef OBSERVERS_AND_API_INC_CONFBUILDER_HPP_
#define OBSERVERS_AND_API_INC_CONFBUILDER_HPP_

#include "Sim.hpp"
#include <string>

class ConfBuilder
{
    public :
        ConfBuilder(const std::string& yaml_model);
        ConfBuilder(const std::string& yaml_model, const VectorOfVectorOfPoints& mesh);

        Sim sim;
        const double Tmax;

};
#endif /* OBSERVERS_AND_API_INC_CONFBUILDER_HPP_ */
