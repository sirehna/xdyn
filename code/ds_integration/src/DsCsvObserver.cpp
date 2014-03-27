#include "DsCsvObserver.hpp"
#include "Observers.hpp"
#include "DsSystem.hpp"

DsCsvObserver::DsCsvObserver(std::ostream& os_) : os(os_), initialized(false)
{}

void DsCsvObserver::observe(const DSSystem& sys, const double t)
{
    const std::vector<double> x = sys.state;
    if (not(initialized))
    {
        const std::vector<std::string> state_names = sys.get_state_names();
        std::vector<std::string> title(1,"t");
        title.insert(title.end(),state_names.begin(),state_names.end());
        serialize(os, title.begin(), title.end());
        initialized = true;
    }
    std::vector<double> v;
    v.push_back(t);
    v.insert(v.end(), x.begin(), x.end());
    serialize(os, v.begin(), v.end());
}
