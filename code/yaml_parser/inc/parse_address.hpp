#ifndef PARSE_ADDRESS_HPP_
#define PARSE_ADDRESS_HPP_

#include <string>
#include "YamlOutput.hpp"

#include <ssc/exception_handling.hpp>
class ParseAddressException: public ::ssc::exception_handling::Exception
{
    public:
        ParseAddressException(const char* s) :
            ::ssc::exception_handling::Exception(s)
        {
        }
};

bool parseWebSocketURL(const std::string& url);
YamlOutput build_YamlOutput_from_WS_URL(const std::string& address);

#endif
