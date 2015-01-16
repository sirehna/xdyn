#ifndef H5_ELEMENT_HPP
#define H5_ELEMENT_HPP

#include "H5Cpp.h"

struct H5Element
{
    H5::DataType datatype;
    H5::DataSpace dataspace;
    H5::DataSet dataset;
    H5Element(): datatype(),dataspace(), dataset(){}
};

#endif
