#include "parse_address.hpp"
#include <cstdlib> // std::atoi
#include <iostream>
#include <boost/regex.hpp>

boost::regex generateBoostRegexParsingWebSocketUrl();
boost::regex generateBoostRegexParsingWebSocketUrl()
{
    return boost::regex("(ws|wss)://([^/ :]+):?([^/ ]*)(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");
}

bool parseWebSocketURL(const std::string& url)
{
    boost::regex ex = generateBoostRegexParsingWebSocketUrl();
    boost::cmatch what;
    if(regex_match(url.c_str(), what, ex))
    {
        std::cout << "protocol: " <<std::string(what[1].first, what[1].second) << std::endl;
        std::cout << "domain:   " <<std::string(what[2].first, what[2].second) << std::endl;
        std::cout << "port:     " <<std::string(what[3].first, what[3].second) << std::endl;
        std::cout << "path:     " <<std::string(what[4].first, what[4].second) << std::endl;
        std::cout << "query:    " <<std::string(what[5].first, what[5].second) << std::endl;
        std::cout << "fragment: " <<std::string(what[6].first, what[6].second) << std::endl;
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * \brief Create a YamlOutput instance from a websocket URL
 * \param url String representing the URL
 * \return YamlOutput
 * \note One uses Boost Regex to parse URL
 *
 * If we want to parse more complex URL, like this one
 * "https://John:Dow@github.com:80/corporateshark/LUrlParser/?&query=ssl#q=frag";
 *
 * one should take a loot at LUrlParser
 * https://github.com/corporateshark/LUrlParser
 * http://130.66.124.6/svn/tools/ThirdParty/LUrlParser
 * \code
 *
 * \endcode
 */
YamlOutput build_YamlOutput_from_WS_URL(const std::string& url)
{
    YamlOutput out;
    boost::regex ex = generateBoostRegexParsingWebSocketUrl();
    boost::cmatch what;
    if(regex_match(url.c_str(), what, ex))
    {
        const std::string protocol(what[1].first, what[1].second);
        const std::string domain(what[2].first, what[2].second);
        const std::string portAsString(what[3].first, what[3].second);
        out.format = protocol;
        if (out.format!="ws")
        {
            std::stringstream ss;
            ss << "Only 'ws' is valid. wss is not implemented yet";
            THROW(__PRETTY_FUNCTION__, ParseAddressException, ss.str());
        }
        out.address = domain;
        const int portAsInt = std::atoi(portAsString.c_str());
        if ( portAsInt <= 0 || portAsInt > 65535 )
        {
            std::stringstream ss;
            ss << "Port: " << portAsInt << " is not valid ";
            THROW(__PRETTY_FUNCTION__, ParseAddressException, ss.str());
        }
        out.port = (short unsigned int)portAsInt;
    }
    else
    {
        std::stringstream ss;
        ss << "Address: " << url << " is not valid ";
        THROW(__PRETTY_FUNCTION__, ParseAddressException, ss.str());
    }
    return out;
}
