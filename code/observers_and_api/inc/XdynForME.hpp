/*
 * XdynForME.hpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#ifndef OBSERVERS_AND_API_INC_XDYNFORME_HPP_
#define OBSERVERS_AND_API_INC_XDYNFORME_HPP_

#include <string>

#include "ConfBuilder.hpp"
#include "HistoryParser.hpp"

class XdynForME
{
    public :
        XdynForME(const std::string& yaml_model);
        std::string calculate_dx_dt(const std::string& raw_yaml);

    private :
        XdynForME();
        ConfBuilder builder;
};



#endif /* OBSERVERS_AND_API_INC_XDYNFORME_HPP_ */
