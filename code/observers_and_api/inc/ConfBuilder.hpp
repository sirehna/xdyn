/*
 * ConfBuilder.hpp
 *
 *  Created on: 22 oct. 2015
 *      Author: sirehna
 */

#ifndef OBSERVERS_AND_API_INC_CONFBUILDER_HPP_
#define OBSERVERS_AND_API_INC_CONFBUILDER_HPP_

#include "Sim.hpp"


class ConfBuilder
{
	public :
	ConfBuilder(const std::string& yaml, const std::map<std::string,double>& commands);

	const Sim sim;
	const double Tmax;

};



#endif /* OBSERVERS_AND_API_INC_CONFBUILDER_HPP_ */
