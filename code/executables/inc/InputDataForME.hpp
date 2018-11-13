/*
 * InputData.hpp
 *
 *  Created on: Nov 13, 2018
 *      Author: cady
 */

#ifndef INPUTDATAFORME_HPP_
#define INPUTDATAFORME_HPP_

#include <string>
#include <vector>

struct InputDataForME
{
    InputDataForME();
    std::vector<std::string> yaml_filenames;
    bool catch_exceptions;
    short unsigned int port;
    bool empty() const;
};


#endif /* INPUTDATAFORME_HPP_ */
