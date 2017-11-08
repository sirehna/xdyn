#include "h5_tools.hpp"

#include "InternalErrorException.hpp"
#include "h5_tools.h"

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
        const H5::H5File& file,
        const std::string& datasetName,
        const H5::DataType& datasetType,
        const H5::DataSpace& space)
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

void H5_Tools::write(
        const std::string& filename,
        const std::string& datasetName,
        const std::string& stringToWrite)
{
    if (not h5_doesFileExists(filename.c_str())) H5_Tools::createEmptyHdf5File(filename);
    H5::H5File f(filename, H5F_ACC_RDWR);
    H5_Tools::write(f, datasetName, stringToWrite);
    f.close();
}

void H5_Tools::write(
        const H5::H5File& file,
        const std::string& datasetName,
        const std::string& stringToWrite)
{
    const hsize_t numberOfLines[1] = {1};
    const size_t sizeStringToWrite = stringToWrite.size();
    if (sizeStringToWrite==0) return;
    H5::StrType strdatatype(H5::PredType::C_S1, sizeStringToWrite);
    H5::DataSpace dataspace(1, numberOfLines);
    H5::DataSet dataset = H5_Tools::createDataSet(file, datasetName, strdatatype, dataspace);
    dataset.write((void*)stringToWrite.c_str(), strdatatype);
    dataset.close();
    dataspace.close();
}

void H5_Tools::write(
        const H5::H5File& file,
        char const * const datasetName,
        char const * const stringToWrite,
        const size_t sizeStringToWrite)
{
    const hsize_t numberOfLines[1] = {1};
    if (sizeStringToWrite==0) return;
    H5::StrType strdatatype(H5::PredType::C_S1, sizeStringToWrite);
    H5::DataSpace dataspace(1, numberOfLines);
    H5::DataSet dataset = H5_Tools::createDataSet(file, datasetName, strdatatype, dataspace);
    dataset.write((void*)stringToWrite, strdatatype);
    dataset.close();
    dataspace.close();
}

void H5_Tools::write(
        const std::string& filename,
        const std::string& datasetName,
        const double& v)
{
    if (not h5_doesFileExists(filename.c_str())) H5_Tools::createEmptyHdf5File(filename);
    H5::H5File f(filename, H5F_ACC_RDWR);
    H5_Tools::write(f, datasetName, v);
    f.close();
}

void H5_Tools::write(
        const H5::H5File& file,
        const std::string& datasetName,
        const double& v)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[2];
    dimsf[0] = 1;
    dimsf[1] = 1;
    H5::DataSpace dataspace(2, dimsf);
    H5::DataSet dataset = H5_Tools::createDataSet(file, datasetName, datatype, dataspace);
    dataset.write(&v, H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void H5_Tools::write(
        const std::string& filename,
        const std::string& datasetName,
        const std::vector<double>& v)
{
    if (not h5_doesFileExists(filename.c_str())) H5_Tools::createEmptyHdf5File(filename);
    H5::H5File f(filename, H5F_ACC_RDWR);
    H5_Tools::write(f, datasetName, v);
    f.close();
}

void H5_Tools::write(
        const H5::H5File& file,
        const std::string& datasetName,
        const std::vector<double>& v)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[2];
    dimsf[0] = (hsize_t)v.size();
    dimsf[1] = 1;
    H5::DataSpace dataspace(2, dimsf);
    H5::DataSet dataset = H5_Tools::createDataSet(file, datasetName, datatype, dataspace);
    dataset.write(&v[0], H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T> >& v);


template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T> >& v)
{
    std::size_t total_size = 0;
    for (const auto& sub : v)
        total_size += sub.size();
    std::vector<T> result;
    result.reserve(total_size);
    for (const auto& sub : v)
        result.insert(result.end(), sub.begin(), sub.end());
    return result;
}

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<std::vector<T> > >& v);
template <typename T>
std::vector<T> flatten(const std::vector<std::vector<std::vector<T> > >& v)
{
    std::size_t total_size = 0;
    for (const auto& sub1 : v)
        for (const auto& sub2 : sub1)
            total_size += sub2.size();
    std::vector<T> result;
    result.reserve(total_size);
    for (const auto& sub1 : v)
        for (const auto& sub2 : sub1)
            result.insert(result.end(), sub2.begin(), sub2.end());
    return result;
}

void H5_Tools::write(
        const std::string& filename,
        const std::string& datasetName,
        const std::vector<std::vector<double> >& v)
{
    if (not h5_doesFileExists(filename.c_str())) H5_Tools::createEmptyHdf5File(filename);
    H5::H5File f(filename, H5F_ACC_RDWR);
    H5_Tools::write(f, datasetName, v);
    f.close();
}

void H5_Tools::write(
        const H5::H5File& file,
        const std::string& datasetName,
        const std::vector<std::vector<double> >& v)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[2];
    dimsf[0] = (hsize_t)v.size();
    dimsf[1] = (hsize_t)v.at(0).size();
    H5::DataSpace dataspace(2, dimsf);
    H5::DataSet dataset = H5_Tools::createDataSet(file, datasetName, datatype, dataspace);
    const auto vflat = flatten(v);
    dataset.write(&vflat[0], H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void H5_Tools::write(
        const std::string& filename,
        const std::string& datasetName,
        const std::vector<std::vector<std::vector<double> > >& v)
{
    if (not h5_doesFileExists(filename.c_str())) H5_Tools::createEmptyHdf5File(filename);
    H5::H5File f(filename, H5F_ACC_RDWR);
    H5_Tools::write(f, datasetName, v);
    f.close();
}

void H5_Tools::write(
        const H5::H5File& file,
        const std::string& datasetName,
        const std::vector<std::vector<std::vector<double> > >& v)
{
    H5::DataType datatype(H5::PredType::NATIVE_DOUBLE);
    hsize_t dimsf[3];
    dimsf[0] = (hsize_t)v.size();
    dimsf[1] = (hsize_t)v.at(0).size();
    dimsf[2] = (hsize_t)v.at(0).at(0).size();
    H5::DataSpace dataspace(3, dimsf);
    H5::DataSet dataset = H5_Tools::createDataSet(file, datasetName, datatype, dataspace);
    const auto vflat = flatten(v);
    dataset.write(&vflat[0], H5::PredType::NATIVE_DOUBLE);
    dataset.close();
    dataspace.close();
}

void H5_Tools::read(
        const std::string& filename,
        const std::string& datasetName,
        double& res)
{
    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5_Tools::read(file, datasetName, res);
    file.close();
}

void H5_Tools::read(
        const std::string& filename,
        const std::string& datasetName,
        std::vector<double> & res)
{
    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5_Tools::read(file, datasetName, res);
    file.close();
}

void H5_Tools::read(
        const std::string& filename,
        const std::string& datasetName,
        std::vector<std::vector<double> >  & res)
{
    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5_Tools::read(file, datasetName, res);
    file.close();
}

void H5_Tools::read(
        const std::string& filename,
        const std::string& datasetName,
        std::vector<std::vector<std::vector<double> > > & res)
{
    H5::H5File file(filename, H5F_ACC_RDONLY);
    H5_Tools::read(file, datasetName, res);
    file.close();
}

void H5_Tools::read(
        const H5::H5File& file,
        const std::string& datasetName,
        double& res)
{
    H5::DataSet dataset = file.openDataSet(datasetName);
    H5T_class_t type_class = dataset.getTypeClass();
    if(type_class != H5T_FLOAT)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects float data");
    }
    const H5::FloatType floattype= dataset.getFloatType();
    std::string order_string;
    H5T_order_t order = floattype.getOrder(order_string);
    if (order != H5T_ORDER_LE)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects little endian data, not " << order_string);
    }
    const size_t size = floattype.getSize();
    if (size!=8)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 64 bit float");
    }
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    if (!((rank==1) || (rank==2)))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 1 or 2 rank data, not" << rank);
    }
    hsize_t dims_out[2];
    int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);
    if (!((ndims==1)||(ndims==2)))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 1 or 2 rank data, not" << ndims);
    }
    H5::DataSpace memspace(1, dims_out);
    dataset.read(&res, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
    dataset.close();
    dataspace.close();
}

void H5_Tools::read(
        const H5::H5File& file,
        const std::string& datasetName,
        std::vector<double> & res)
{
    H5::DataSet dataset = file.openDataSet(datasetName);
    H5T_class_t type_class = dataset.getTypeClass();
    if(type_class != H5T_FLOAT)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects float data");
    }
    const H5::FloatType floattype= dataset.getFloatType();
    std::string order_string;
    H5T_order_t order = floattype.getOrder(order_string);
    if (order != H5T_ORDER_LE)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects little endian data, not " << order_string);
    }
    const size_t size = floattype.getSize();
    if (size!=8)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 64 bit float");
    }
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    if (!((rank==1) || (rank==2)))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 1 or 2 rank data, not" << rank);
    }
    hsize_t dims_out[2];
    int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);
    if (!((ndims==1)||(ndims==2)))
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 1 or 2 rank data, not" << ndims);
    }
    H5::DataSpace memspace(1, dims_out);
    double *p = (double*)malloc((size_t) dims_out[0] * sizeof(double));
    dataset.read(p, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
    res = std::vector<double>(p, p + dims_out[0]);
    free(p);
    dataset.close();
    dataspace.close();
}

void H5_Tools::read(
        const H5::H5File& file,
        const std::string& datasetName,
        std::vector<std::vector<double> > & res)
{
    H5::DataSet dataset = file.openDataSet(datasetName);
    H5T_class_t type_class = dataset.getTypeClass();
    if(type_class != H5T_FLOAT)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects float data");
    }
    const H5::FloatType floattype= dataset.getFloatType();
    std::string order_string;
    H5T_order_t order = floattype.getOrder(order_string);
    if (order != H5T_ORDER_LE)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects little endian data, not " << order_string);
    }
    const size_t size = floattype.getSize();
    if (size!=8)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 64 bit float");
    }
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    if (rank!=2)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 2 rank data, not" << rank);
    }
    hsize_t dims_out[2];
    int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);
    if (ndims!=2)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 2 rank data, not" << ndims);
    }
    H5::DataSpace memspace(2, dims_out);
    double *p = (double*)malloc((size_t) (dims_out[0] * dims_out[1]) * sizeof(double));
    dataset.read(p, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
    for (hsize_t i=0; i<dims_out[0];++i)
    {
        res.push_back(std::vector<double>(p + i *dims_out[1], p + (i + 1) * dims_out[1]));
    }
    free(p);
    dataset.close();
    dataspace.close();
}

void H5_Tools::read(
        const H5::H5File& file,
        const std::string& datasetName,
        std::vector<std::vector<std::vector<double> > > & res)
{
    H5::DataSet dataset = file.openDataSet(datasetName);
    H5T_class_t type_class = dataset.getTypeClass();
    if(type_class != H5T_FLOAT)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects float data");
    }
    const H5::FloatType floattype= dataset.getFloatType();
    std::string order_string;
    H5T_order_t order = floattype.getOrder(order_string);
    if (order != H5T_ORDER_LE)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects little endian data, not " << order_string);
    }
    const size_t size = floattype.getSize();
    if (size!=8)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 64 bit float");
    }
    H5::DataSpace dataspace = dataset.getSpace();
    int rank = dataspace.getSimpleExtentNdims();
    if (rank!=3)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 3 rank data, not" << rank);
    }
    hsize_t dims_out[3];
    int ndims = dataspace.getSimpleExtentDims(dims_out, NULL);
    if (ndims!=3)
    {
        THROW(__PRETTY_FUNCTION__, InternalErrorException, "One expects 3 rank data, not" << ndims);
    }
    H5::DataSpace memspace(3, dims_out);
    double *p = (double*)malloc((size_t) (dims_out[0] * dims_out[1] * dims_out[2]) * sizeof(double));
    dataset.read(p, H5::PredType::NATIVE_DOUBLE, memspace, dataspace);
    for (hsize_t i=0; i<dims_out[0];++i)
    {
        std::vector<std::vector<double> > tmp;
        for (hsize_t j=0; j<dims_out[1]; ++j)
        {
            tmp.push_back(std::vector<double>(p + i *dims_out[1] * dims_out[2] + j * dims_out[2],
                                              p + i *dims_out[1] * dims_out[2] + (j+1) * dims_out[2]));
        }
        res.push_back(tmp);
    }
    free(p);
    dataset.close();
    dataspace.close();
}

