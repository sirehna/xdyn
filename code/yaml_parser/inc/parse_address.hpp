#ifndef PARSE_ADDRESS_HPP_
#define PARSE_ADDRESS_HPP_

#include <string>
#include "YamlOutput.hpp"

#include <ssc/exception_handling.hpp>
class ParseAddressException: public ::ssc::exception_handling::Exception
{
    public:
        ParseAddressException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};

bool parseWebSocketURL(const std::string& url);
YamlOutput build_YamlOutput_from_WS_URL(const std::string& address);

#endif
