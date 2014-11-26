#include <ssc/exception_handling.hpp>

class HistoryException: public ssc::exception_handling::Exception
{
    public:
        HistoryException(const char* s) :
                Exception(s)
        {
        }
};
