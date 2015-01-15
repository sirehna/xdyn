/*
 * Hdf5Observer.cpp
 *
 *  Created on: Jan 12, 2015
 *      Author: cady
 */

#include "h5_version.hpp"
#include "h5_tools.hpp"
#include "Hdf5Observer.hpp"

#include "demoMatLab.hpp"
#include "demoPython.hpp"

Hdf5Observer::Hdf5Observer(
        const std::string& filename,
        const std::vector<std::string>& d) :
            Observer(d), h5File(H5::H5File(filename,H5F_ACC_TRUNC)), basename("outputs"),datasetNames(),h5Dataset()
{
    h5_writeFileDescription(h5File);
    exportMatLabScripts(h5File, filename, basename, basename.empty()?"scripts/MatLab":basename+"/scripts/MatLab");
    exportPythonScripts(h5File, filename, basename, basename.empty()?"scripts/Python":basename+"/scripts/Python");
}

std::function<void()> Hdf5Observer::get_serializer(const double, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::string&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::vector<double>&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_serializer(const std::vector<std::vector<double> >&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const double , const std::vector<std::string>& g, const std::vector<std::string>&, const std::string&)
{
    return [this,g](){datasetNames.insert(H5_Tools::ensureStringStartsAndEndsWithAPattern(H5_Tools::join(g,"/"),"/"));};
}

std::function<void()> Hdf5Observer::get_initializer(const std::string&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string&)
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::vector<double>&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

std::function<void()> Hdf5Observer::get_initializer(const std::vector<std::vector<double> >&, const std::vector<std::string>&, const std::vector<std::string>&, const std::string& )
{
    return [](){};
}

void Hdf5Observer::flush_after_initialization()
{
    for (const auto datasetName:datasetNames)
    {
        h5Dataset[datasetName] = H5::DataSet();
    }
}

void Hdf5Observer::flush_after_write()
{
}

void Hdf5Observer::flush_value()
{
}
