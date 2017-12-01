/*
 * YamlHOS.hpp
 *
 *  Created on: Nov 29, 2017
 *      Author: cady
 */

#ifndef EXTERNAL_DATA_STRUCTURES_INC_YAMLHOS_HPP_
#define EXTERNAL_DATA_STRUCTURES_INC_YAMLHOS_HPP_

#include <string>

struct YamlHOS
{
    enum class ErrorType {ABSOLUTE, RELATIVE};
    YamlHOS();
    int n1;
    int n2;
    int m;
    int p1;
    int p2;
    ErrorType err;
    float toler;
    float xlen;
    float ylen;
    float depth;
    float gamma;
    float beta;
    float tp_real;
    float hs_real;
    std::string address_brokerHOS;
};



#endif /* EXTERNAL_DATA_STRUCTURES_INC_YAMLHOS_HPP_ */