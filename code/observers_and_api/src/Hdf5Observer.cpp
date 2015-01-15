#include "h5_version.hpp"
#include "h5_tools.hpp"
#include "Hdf5Observer.hpp"

#include "demoMatLab.hpp"
#include "demoPython.hpp"

Hdf5Addressing::Hdf5Addressing(
        const DataAddressing& addressing,
        const std::string& basename) :
            name(addressing.name),
            address(H5_Tools::ensureStringStartsWithAPattern(basename,"/") +
                    H5_Tools::ensureStringStartsWithAPattern(H5_Tools::join(addressing.address,"/"),"/"))
{
}

Hdf5Observer::Hdf5Observer(
        const std::string& filename,
        const std::vector<std::string>& d) :
            Observer(d), h5File(H5::H5File(filename,H5F_ACC_TRUNC)), basename("outputs"),address2columns(),address2dataset(),name2datatype()
{
    h5_writeFileDescription(h5File);
    exportMatLabScripts(h5File, filename, basename, "/scripts/MatLab");
    exportPythonScripts(h5File, filename, basename, "/scripts/Python");
}

std::function<void()> Hdf5Observer::get_serializer(const double val, const DataAddressing& addressing)
{
    return [this,val,addressing]()
           {
                const Hdf5Addressing hdf5_addressing(addressing,this->basename);
                H5::DataSet dataset = address2dataset[hdf5_addressing.address];
                H5::DataSpace dataspace = dataset.getSpace();
                hsize_t offset[1];
                hsize_t size[1];
                if (dataspace.getSimpleExtentDims(size)!=1)
                {
                    //throw rank exception
                }
                const hsize_t dims[1] = {(hsize_t)1};
                offset[0] = size[0];
                size[0] += 1;
                dataset.extend(size);
                H5::DataSpace fspace = dataset.getSpace();
                fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
                dataset.write(&val, name2datatype[addressing.name], H5_Tools::createDataSpace1DUnlimited(), fspace);
           };
}

std::function<void()> Hdf5Observer::get_initializer(const double , const DataAddressing& addressing)
{
    return [this,addressing]()
           {
                const Hdf5Addressing hdf5_addressing(addressing,this->basename);
                auto it = address2columns.find(hdf5_addressing.address);
                if (it != address2columns.end()) it->second.push_back(addressing);
                else                             address2columns[hdf5_addressing.address] = std::vector<Hdf5Addressing>(1,hdf5_addressing);
                name2datatype[addressing.name] = H5::DataType(H5::PredType::NATIVE_DOUBLE);
           };
}

void Hdf5Observer::flush_after_initialization()
{
    for (const auto addressing:address2columns)
    {
        hsize_t dimsT[1] = {1};
        const hsize_t maxdimsT[1] = {H5S_UNLIMITED};
        H5::DataSpace dataspace(1, dimsT, maxdimsT);
        if (addressing.second.size()==1)
        {
            address2dataset[addressing.first] = H5_Tools::createDataSet(h5File, addressing.first, name2datatype[addressing.second.front().name], dataspace);
        }
        else
        {
            /* No to create a dynamic coumpound datatype*/
            //throw should not append
        }
    }
}

void Hdf5Observer::flush_after_write()
{
}

void Hdf5Observer::flush_value()
{
}
