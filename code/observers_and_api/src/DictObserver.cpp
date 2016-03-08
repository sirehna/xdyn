#include "DictObserver.hpp"
#include "base91.hpp"
#include <iostream>
#include <utility>

typedef std::pair<std::string,std::string> DictMapKeyVar;
DictMapKeyVar extractKeyVarFromString(const std::string& s);
DictMapKeyVar extractKeyVarFromString(const std::string& s)
{
    DictMapKeyVar j;
    auto kS = s.find('(');
    auto kE = s.find_last_of(')');
    if ((kS == std::string::npos) and (kE == std::string::npos))
    {
        j.first = s;
    }
    else
    {
        j.first = s.substr(kS+1,kE-kS-1);
        j.second = s.substr(0,kS);
    }
    return j;
}

DictObserver::DictObserver(const std::vector<std::string>& d) :
        Observer(d), ss(), ssSurfaceElevationGrid(), dictMap1(), dictMap2(), shouldWeAddAStartingComma(false)
{
}

DictObserver::~DictObserver()
{
}

std::function<void()> DictObserver::get_serializer(const double val, const DataAddressing& d)
{
    return [this,d,val]()
                      {
                        DictMapKeyVar j = extractKeyVarFromString(d.name);
                        if (not j.first.empty())
                        {
                            if (j.second.empty())
                            {
                                dictMap1[j.first] = val;
                            }
                            else
                            {
                                dictMap2[j.first][j.second] = val;
                            }
                        }
                      };
}

std::function<void()> DictObserver::get_initializer(const double, const DataAddressing&)
{
    return [this](){};
}

std::function<void()> DictObserver::get_serializer(const SurfaceElevationGrid& s, const DataAddressing&)
{
    return [this,s](){
        const size_t nx = (size_t)s.x.size();
        const size_t ny = (size_t)s.y.size();
        const size_t n = (size_t)s.z.size();
        std::vector<float> v(n,0.0);
        double const * const data = s.z.data();
        for (size_t i=0;i<n;++i) v[i] = (float)data[i];
        ssSurfaceElevationGrid.str(std::string());
        if (n>0) ssSurfaceElevationGrid << "\"waves\":{nx : " << nx <<",ny : " << ny << ",z: '"<<base<91>::encode(sizeof(float)*n,&v[0])<<"'}";
    };
}

std::function<void()> DictObserver::get_initializer(const SurfaceElevationGrid&, const DataAddressing&)
{
    return [this](){};
}

void flushMap(std::ostream& os, const std::map<std::string,double>& stuff_to_write, bool addBraces);
void flushMap(std::ostream& os, const std::map<std::string,double>& stuff_to_write, bool addBraces)
{
    const size_t n = stuff_to_write.size();
    if (n==0) return;
    size_t i = 0;
    if (addBraces) os << "{";
    for (auto const& stuff:stuff_to_write)
    {
        os << "\""<<stuff.first<<"\":"<<stuff.second;
        if (i<(n-1)) os << ",";
        ++i;
    }
    if (addBraces) os << "}";
}

bool isStringStreamEmpty(const std::stringstream& ss);
bool isStringStreamEmpty(const std::stringstream& ss)
{
    return ss.rdbuf()->in_avail() == 0;
}

void DictObserver::serializeDictMap1()
{
    const size_t n1 = dictMap1.size();
    if (n1==0) return;
    flushMap(ss, dictMap1, false);
    shouldWeAddAStartingComma = true;
}

void DictObserver::serializeDictMap2Attitudes()
{
    size_t i = 0;
    size_t nAttitudes = 0;
    for (auto const& object:dictMap2)
    {
        if (object.first.find(',')==std::string::npos)
        {
            nAttitudes++;
        }
    }
    i = 0;
    ss << "\"solids\":{";
    for (auto const& object:dictMap2)
    {
        if (object.first.find(',')!=std::string::npos) continue;
        ss << "\""<<object.first<<"\":";
        flushMap(ss, object.second, true);
        if (i<(nAttitudes-1)) {ss << ",";}
        ++i;
    }
    ss << "}";
}

void DictObserver::serializeDictMap2Wrenches()
{
    size_t i = 0;
    size_t nAttitudes = 0, nWrenches = 0;
    for (auto const& object:dictMap2)
    {
        if (object.first.find(',')==std::string::npos)
        {
            nAttitudes++;
        }
        else
        {
            nWrenches++;
        }
    }
    if (nWrenches>0)
    {
        i = 0;
        if (nAttitudes>0) ss<<",";
        ss << "\"wrenches\":{";
        for (auto const& object:dictMap2)
        {
            if (object.first.find(',')==std::string::npos) continue;
            ss << "\""<<object.first<<"\":";
            flushMap(ss, object.second, true);
            if (i<(nWrenches-1)) {ss << ",";}
            ++i;
        }
        ss << "}";
    }
}

void DictObserver::serializeDictMap2()
{
    if (dictMap2.empty()) return;
    if (shouldWeAddAStartingComma) ss << ",";
    serializeDictMap2Attitudes();
    serializeDictMap2Wrenches();
    shouldWeAddAStartingComma = true;
}

void DictObserver::serializeDictSurfaceElevationGrid()
{
    if (isStringStreamEmpty(ssSurfaceElevationGrid)) return;
    if (shouldWeAddAStartingComma) ss<<",";
    ss << ssSurfaceElevationGrid.str();
    shouldWeAddAStartingComma = true;
}

void DictObserver::flush_after_write()
{
    ss << "{";
    serializeDictMap1();
    serializeDictMap2();
    serializeDictSurfaceElevationGrid();
    ss << "}";
}
