#include "InternalErrorException.hpp"
#include "h5_tools.h"
#include "h5_tools.hpp"

std::string H5_Tools::ensureStringStartsAndEndsWithAPattern(
        const std::string & str,        //!< String to check
        const std::string & delim       //!< Pattern to place at the beginning and the end of the \a str
        )
{
    std::string res(str);
    if (str.empty()) return delim;
    if (delim.size()>res.size()) return delim + res + delim;
    if (res.rfind(delim,0)!=0) res = delim + res;
    const size_t lastPoss = res.size()-delim.size();
    if (res.rfind(delim,lastPoss)!=lastPoss) res = res+delim;
    return res;
}

std::string H5_Tools::ensureStringStartsWithAPattern(
        const std::string & str,        //!< String to check
        const std::string & delim       //!< Pattern to place at the beginning of the \a str
        )
{
    std::string res(str);
    if (str.empty()) return delim;
    if (delim.size()>res.size()) return delim + res + delim;
    if (res.rfind(delim,0)!=0) res = delim + res;
    return res;
}

std::vector<std::string> H5_Tools::split(const std::string & str, const std::string & delim)
{
    std::vector<std::string> tokens;
    if (not(str.empty()))
    {
        if (not(delim.empty()))
        {
            std::string::size_type currPos = 0, prevPos = 0;
            while ((currPos = str.find(delim, prevPos)) != std::string::npos)
            {
                std::string item = str.substr(prevPos, currPos - prevPos);
                if (not(item.empty()))
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

std::string H5_Tools::replaceString(
        const std::string& subject,
        const std::string& search,
        const std::string& replace) {
    std::string result(subject);
    size_t pos = 0;
    while ((pos = result.find(search, pos)) != std::string::npos)
    {
         result.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return result;
}

std::string& H5_Tools::replaceStringInPlace(
        std::string& subject,
        const std::string& search,
        const std::string& replace)
{
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos)
    {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

std::string H5_Tools::getBasename(std::string const & path, std::string const & delims)
{
  return path.substr(path.find_last_of(delims) + 1);
}

H5::Group H5_Tools::createMissingGroups(
        const H5::H5File& file,
        const std::string& datasetName)
{
    const std::vector<std::string> tokens = split(datasetName, "/");
    return createMissingGroups(file, tokens);
}

H5::Group H5_Tools::createMissingGroups(
        const H5::H5File& file,
        const std::vector<std::string>& datasetName)
{
    const size_t nTokens = datasetName.size();
    std::string current;
    if (nTokens<(size_t)2)
    {
        return file.openGroup("/");
    }
    else if (nTokens>(size_t)2)
    {
        for (size_t i=0;i<(nTokens-2);++i)
        {
            const std::string& currentGroup = datasetName.at(i);
            current += "/"+currentGroup;
            if (H5Lexists(file.getId(), current.c_str(), H5P_DEFAULT)<=0)
            {
                file.createGroup(current);
            }
        }
    }
    const std::string& currentGroup = datasetName.at(nTokens-2);
    current += "/"+currentGroup;
    if (H5Lexists(file.getId(), current.c_str(), H5P_DEFAULT)<=0)
    {
        return file.createGroup(current);
    }
    else
    {
        return file.openGroup(current);
    }
}

H5::DataSet H5_Tools::createDataSet(
        const H5::H5File& file, const std::string& datasetName,
        const H5::DataType& datasetType, const H5::DataSpace& space)
{
    const int nDims = space.getSimpleExtentNdims();
    hsize_t * chunk_dims = new hsize_t[nDims];
    for (int i=0;i<nDims;++i) chunk_dims[i]=1;
    H5::DSetCreatPropList cparms;
    cparms.setChunk(nDims, chunk_dims);
    delete [] chunk_dims;
    createMissingGroups(file, datasetName);
    if (H5_Tools::doesDataSetExist(file, datasetName))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "When serializing to HDF5, attempting to create dataset '" << datasetName << "' twice");
    }
    else
    {
        return file.createDataSet(datasetName, datasetType, space, cparms);
    }
}

H5::DataSet H5_Tools::openDataSet(
        const H5::H5File& file, const std::string& datasetName)
{
    if (H5_Tools::doesDataSetExist(file, datasetName))
    {
        return file.openDataSet(datasetName);
    }
    else
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "Attempting to open a non existing dataset '" << datasetName << "' twice");
    }
}

bool H5_Tools::doesDataSetExist(
        const H5::H5File& file, const std::string& datasetName)
{
    const htri_t dataset_status = H5Lexists(file.getId(), datasetName.c_str(), H5P_DEFAULT);
    return (dataset_status>0) ? true : false;
}

std::string H5_Tools::join(const std::vector<std::string>&v, const std::string& delim)
{
    std::ostringstream s;
    for (std::vector<std::string>::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        if (it != v.begin())
        {
            s << delim;
        }
        s << *it;
    }
    return s.str();
}

H5::DataSpace H5_Tools::createDataSpace1DEmptyUnlimited()
{
    const hsize_t dims[1] = {0};
    const hsize_t maxdims[1] = {H5S_UNLIMITED};
    return H5::DataSpace(1, dims, maxdims);
}

H5::DataSpace H5_Tools::createDataSpace1DUnlimited()
{
    const hsize_t dims[1] = {1};
    const hsize_t maxdims[1] = {H5S_UNLIMITED};
    return H5::DataSpace(1, dims, maxdims);
}

void H5_Tools::writeString(
        const H5::H5File& file,
        const std::string& datasetName,
        const std::string& stringToWrite)
{
    const hsize_t numberOfLines[1] = {1};
    H5::StrType strdatatype(H5::PredType::C_S1, stringToWrite.size());
    H5::DataSpace sid1(1, numberOfLines);
    H5::DataSet d = H5_Tools::createDataSet(file, datasetName, strdatatype, sid1);
    d.write((void*)stringToWrite.c_str(), strdatatype);
}

void H5_Tools::writeString(
        const std::string& filename,
        const std::string& datasetName,
        const std::string& stringToWrite)
{
    if (not h5_doesFileExists(filename.c_str())) H5_Tools::createEmptyHdf5File(filename);
    H5_Tools::writeString(H5::H5File(filename, H5F_ACC_RDWR), datasetName, stringToWrite);
}

bool H5_Tools::doesFileExists(const std::string& filename)
{
    return (h5_doesFileExists(filename.c_str())?true:false);
}

void H5_Tools::createEmptyHdf5File(const std::string& filename)
{
    H5::H5File file(filename, H5F_ACC_TRUNC);
    file.close();
}

void H5_Tools::ifFileDoesNotExistsCreateOne(const std::string& filename)
{
    if (not H5_Tools::doesFileExists(filename))
    {
        H5_Tools::createEmptyHdf5File(filename);
    }
}

H5::H5File H5_Tools::openOrCreateAHdf5File(const std::string& filename)
{
    H5_Tools::ifFileDoesNotExistsCreateOne(filename);
    return H5::H5File(filename, H5F_ACC_RDWR);
}

H5::H5File H5_Tools::openEmptyHdf5File(const std::string& filename)
{
    return H5::H5File(filename, H5F_ACC_TRUNC);
}
