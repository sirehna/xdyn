#ifndef HDBDATAHPP
#define HDBDATAHPP

#include <string>

namespace test_data
{
    std::string test_ship_hdb();
    double analytical_Br(const double omega);
    double analytical_K(const double tau);
    std::string bug_3210();
    std::string bug_3230_hdb();
    std::string bug_3207_hdb();
    std::string bug_3238_hdb();
}

#endif
