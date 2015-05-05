/*
 * string2vector.hpp
 *
 *  Created on: Apr 30, 2015
 *      Author: cady
 */


#ifndef STRING2VECTOR_HPP_
#define STRING2VECTOR_HPP_

#include <cstring> //memcpy
#include <string>
#include <vector>

template<typename T>
std::string convert_vector_to_string(const std::vector<T>& v)
{
    const size_t ss = v.size()*sizeof(T);
    std::string s(ss, 0);
    memcpy((void*)s.c_str(),v.data(),ss);
    return s;
}

template<typename T>
typename std::vector<T> convert_string_to_vector(const std::string& s)
{
    std::vector<T> res;
    const size_t n = s.size()/sizeof(T);
    res.resize(n);
    for (size_t i=0;i<n;++i)
    {
        res[i]=*reinterpret_cast<const double*>(s.c_str() + sizeof(T)*i);
    }
    return res;
}

#endif  /* STRING2VECTOR_HPP_ */
