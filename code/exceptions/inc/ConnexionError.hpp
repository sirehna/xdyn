/*
 * InternalErrorException.hpp
 *
 *  Created on: Nov 4, 2015
 *      Author: cady
 */


#ifndef CORE_INC_CONNEXION_ERROR_HPP_
#define CORE_INC_CONNEXION_ERROR_HPP_

#include <ssc/exception_handling.hpp>

class ConnexionError: public ssc::exception_handling::Exception
{
    public:
        ConnexionError(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
               Exception(message, file, function, line)
        {
        }
};

#endif  /* CORE_INC_INTERNALERROREXCEPTION_HPP_ */
