/*
 * InputData.cpp
 *
 *  Created on: Jun 24, 2014
 *      Author: cady
 */

#include "InputDataForME.hpp"

InputDataForME::InputDataForME() : yaml_filenames(),
                         catch_exceptions(false),
                         port(0)
{
}

bool InputDataForME::empty() const
{
    return yaml_filenames.empty() and (port == 0);
}


