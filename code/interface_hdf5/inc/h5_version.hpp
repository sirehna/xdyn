#ifndef H5_VERSION_HPP
#define H5_VERSION_HPP

#include <string>
#include "H5Cpp.h"

bool h5_writeFileDescription(
        const std::string& fileName);

bool h5_writeFileDescription(
        const H5::H5File& h5File);

#endif
