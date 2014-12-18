#include "h5_version.h"
#include "h5_version.hpp"

bool h5_writeFileDescription(
        const std::string& fileName)
{
    return h5_writeFileDescription(fileName.c_str())?true:false;
}
