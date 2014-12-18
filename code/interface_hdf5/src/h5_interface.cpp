#include "h5_interface.hpp"
#include <vector>

inline std::vector<std::string> Split(const std::string & str, const std::string & delim = "/")
{
    std::vector<std::string> tokens;
    if (str.size() > 0)
    {
        if (delim.size() > 0)
        {
            std::string::size_type currPos = 0, prevPos = 0;
            while ((currPos = str.find(delim, prevPos)) != std::string::npos)
            {
                std::string item = str.substr(prevPos, currPos - prevPos);
                if (item.size() > 0)
                {
                    tokens.push_back(item);
                }
                prevPos = currPos + 1;
            }
            tokens.push_back(str.substr(prevPos));
        }
        else
        {
            tokens.push_back(str);
        }
    }
    return tokens;
}

inline void createMissingGroups(const H5::H5File& file, const std::string& datasetName)
{
    const std::vector<std::string> tokens = Split(datasetName, "/");
    std::string current;
    for (std::size_t i=0;i<(tokens.size()-1);++i)
    {
        const std::string& currentGroup = tokens.at(i);
        current += "/"+currentGroup;
        if (H5Lexists(file.getId(), current.c_str(), H5P_DEFAULT)<=0)
        {
            file.createGroup(current);
        }
    }
}

H5::DataSet h5_CreateDataSet(const H5::H5File& file, const std::string& datasetName, const H5::DataType& datasetType, const H5::DataSpace& space)
{
    const hsize_t chunk_dims[1] = {1};
    H5::DSetCreatPropList cparms;
    cparms.setChunk(1, chunk_dims);
    if (datasetName.find('/')!=std::string::npos)
    {
        createMissingGroups(file, datasetName);
    }
    htri_t dataset_status = H5Lexists(file.getId(), datasetName.c_str(), H5P_DEFAULT);
    if (dataset_status<=0)
    {
        return file.createDataSet(datasetName, datasetType, space, cparms);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, H5InterfaceException, "Dataset already exists");
        return file.openDataSet(datasetName);
    }
}

H5::DataSpace h5_CreateDataSpace1DUnlimited()
{
    const hsize_t dims[1] = {1};
    const hsize_t maxdims[1] = {H5S_UNLIMITED};
    return H5::DataSpace(1, dims, maxdims);
}
