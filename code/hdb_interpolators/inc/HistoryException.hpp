#include <ssc/exception_handling.hpp>

class HistoryException: public ssc::exception_handling::Exception
{
    public:
        HistoryException(const std::string& message, const std::string& file, const std::string& function, const unsigned int line) :
            ::ssc::exception_handling::Exception(message, file, function, line)
        {
        }
};
