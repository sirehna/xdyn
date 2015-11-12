#ifndef H5_INTERFACE_HPP
#define H5_INTERFACE_HPP

#include <string>
#include <vector>
#include "H5Cpp.h"
#include "h5_tools.hpp"

template <typename T> class H5_Interface
{
    public:
        H5_Interface():
                h5Space(H5_Tools::createDataSpace1DUnlimited()),
                h5Type(createId())
        {
        }
        H5_Interface(const H5::CompType& h5Type_):
                h5Space(H5_Tools::createDataSpace1DUnlimited()),
                h5Type(h5Type_)
        {
        }
        virtual ~H5_Interface(){}
    protected:
        H5::CompType get_type() const
        {
            return h5Type;
        }
        H5::DataSpace get_space() const
        {
            return h5Space;
        }
    private:
        H5::CompType createId();
    protected:
        H5::DataSpace   h5Space;
        H5::CompType    h5Type;
};

template <typename T> class H5_Serialize : public H5_Interface<T>
{
    public:
        H5_Serialize(
                const std::string& fileName,
                const std::string& datasetName): H5_Interface<T>(),
                    file(H5::H5File(fileName, H5F_ACC_TRUNC)),
                    dataset(H5_Tools::createDataSet(file,datasetName,this->get_type(),this->get_space())),
                    n(0),inputIsDirectlyAH5File(false)
        {
        }
        H5_Serialize(
                const H5::H5File& h5File,
                const std::string& datasetName): H5_Interface<T>(),
                    file(h5File),
                    dataset(H5_Tools::createDataSet(file,datasetName,this->get_type(),this->get_space())),
                    n(0),inputIsDirectlyAH5File(true)
        {
        }

        H5_Serialize(
                const std::string& fileName,
                const std::string& datasetName,
                const H5::CompType& h5Type): H5_Interface<T>(h5Type),
                    file(H5::H5File(fileName, H5F_ACC_TRUNC)),
                    dataset(H5_Tools::createDataSet(file,datasetName,this->get_type(),this->get_space())),
                    n(0),inputIsDirectlyAH5File(false)
        {
        }
        H5_Serialize(
                const H5::H5File& h5File,
                const std::string& datasetName,
                const H5::CompType& h5Type): H5_Interface<T>(h5Type),
                    file(h5File),
                    dataset(H5_Tools::createDataSet(file,datasetName,this->get_type(),this->get_space())),
                    n(0),inputIsDirectlyAH5File(true)
        {
        }

        ~H5_Serialize()
        {
            H5_Interface<T>::h5Type.close();
            H5_Interface<T>::h5Space.close();
            if(!inputIsDirectlyAH5File)
            {
                file.close();
            }
        }

        void write(T const * const data)
        {
            const hsize_t dims[1] = {(hsize_t)1};
            hsize_t offset[1];
            hsize_t size[1];
            offset[0] = n;
            size[0] = ++n;
            dataset.extend(size);
            H5::DataSpace fspace = dataset.getSpace();
            fspace.selectHyperslab(H5S_SELECT_SET, dims, offset);
            dataset.write(data, this->get_type(), this->get_space(), fspace);
        }

        H5_Serialize<T>& operator<<(
                            const T& obj)
        {
            write(&obj);
            return *this;
        }

    private:
        H5_Serialize();
        H5::H5File      file;                   /**< HDF5 file pointer*/
        H5::DataSet     dataset;                /**< HDF5 dataset*/
        hsize_t         n;                      /**< Counter to manage offset*/
        bool            inputIsDirectlyAH5File; /**< Boolean used for destructor*/
};

template <typename T> class H5_Parse : public H5_Interface<T>
{
    public:
        H5_Parse(): H5_Interface<T>()
        {
        }

        ~H5_Parse()
        {
            H5_Interface<T>::h5Type.close();
            H5_Interface<T>::h5Space.close();
        }

        std::vector<T> Read(
                const H5::H5File& h5file,
                const std::string& datasetName)
        {
            const H5::DataSet dataset = h5file.openDataSet(datasetName);
            const H5::DataSpace dataspace = dataset.getSpace();
            const std::size_t ndims = (std::size_t)dataspace.getSimpleExtentNdims();
            if (ndims != 1)
            {
                //throw std::runtime_error("HDF5 array has incorrect number of dimensions to represent a sparse matrix.");
            }
            hsize_t nnz;
            dataspace.getSimpleExtentDims(&nnz);
            std::vector<T> res((std::size_t)nnz);
            dataset.read(res.data(), this->get_type(), dataspace);
            return res;
        }

        std::vector<T> Read(
                const std::string& FileName,
                const std::string& datasetName)
        {
            H5::H5File file(FileName, H5F_ACC_RDONLY);
            std::vector<T> res = Read(file,datasetName);
            file.close();
            return res;
        }
};

#endif
