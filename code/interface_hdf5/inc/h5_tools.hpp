#ifndef H5_TOOLS_HPP
#define H5_TOOLS_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "H5Cpp.h"

namespace H5_Tools
{
    std::vector<std::string> split(
            const std::string & str,
            const std::string & delim = "/");

    std::string ensureStringStartsAndEndsWithAPattern(
            const std::string & str,
            const std::string & delim);

    std::string ensureStringStartsWithAPattern(
            const std::string & str,
            const std::string & delim
            );

    std::string getBasename(
            const std::string& path,
            const std::string& delims = "/\\");

    std::string replaceString(
            const std::string& subject,
            const std::string& search,
            const std::string& replace);

    std::string& replaceStringInPlace(
            std::string& subject,
            const std::string& search,
            const std::string& replace);

    std::string join(const std::vector<std::string>&v, const std::string& delim);

    H5::DataSpace createDataSpace1DEmptyUnlimited();

    H5::DataSpace createDataSpace1DUnlimited();

    /**
     * \brief creates a dataset, if not existing. Else, this function throws an
     *        exception
     * \param[in] file HDF5 file descriptor
     * \param[in] datasetName Location of the dataset. May contain /,
     *                        indicating groups
     * \param[in] datasetType dataset type
     * \param[in] space dataset space
     * \return the dataset descriptor
     */
    H5::DataSet createDataSet(
            const H5::H5File& file, const std::string& datasetName,
            const H5::DataType& datasetType, const H5::DataSpace& space);

    /**
     * \brief open an existing data set
     * \param[in] file HDF5 file descriptor
     * \param[in] datasetName Location of the dataset. May contain /,
     *                        indicating groups
     * \return the dataset
     */
    H5::DataSet openDataSet(
            const H5::H5File& file, const std::string& datasetName);

    /**
     * \brief checks if a dataset already exists in a HDF5 file
     * \param[in] file HDF5 file descriptor
     * \param[in] datasetName Location of the dataset. May contain /,
     *                        indicating groups
     * @return boolean indicating whether the dataset existing
     */
    bool doesDataSetExist(
            const H5::H5File& file, const std::string& datasetName);

    H5::Group createMissingGroups(
            const H5::H5File& file,
            const std::string& datasetName);

    H5::Group createMissingGroups(
            const H5::H5File& file,
            const std::vector<std::string>& datasetName);

    /**
     * \brief writes a string in a HDF5 file.
     * \note This function creates the HDF5 if not existing
     * \param[in] filename Hdf5 filename
     * \param[in] datasetName Data set where to write the string content.
     *            Can use / to create group
     * \param[in] stringToWrite
     */
    void write(
            const std::string& filename,
            const std::string& datasetName,
            const std::string& stringToWrite);

    void write(
            const H5::H5File& file,
            const std::string& datasetName,
            const std::string& stringToWrite);

    void write(
            const H5::H5File& file,
            char const * const datasetName,
            char const * const stringToWrite,
            const size_t sizeStringToWrite);

    void write(
            const H5::H5File& file,
            const std::string& datasetName,
            const std::string& stringToWrite);

    void write(
            const H5::H5File& file,
            char const * const datasetName,
            char const * const stringToWrite,
            const size_t sizeStringToWrite);

    void write(
            const std::string& filename,
            const std::string& datasetName,
            const double& v);

    void write(
            const H5::H5File& group,
            const std::string& datasetName,
            const double& v);

    void write(
            const std::string& filename,
            const std::string& datasetName,
            const std::vector<double>& v);

    void write(
            const H5::H5File& file,
            const std::string& datasetName,
            const std::vector<double>& v);

    void write(
            const std::string& filename,
            const std::string& datasetName,
            const std::vector<std::vector<double> >& v);

    void write(
            const H5::H5File& file,
            const std::string& datasetName,
            const std::vector<std::vector<double> >& v);

    void write(
            const std::string& filename,
            const std::string& datasetName,
            const std::vector<std::vector<std::vector<double> > >& v);

    void write(
            const H5::H5File& file,
            const std::string& datasetName,
            const std::vector<std::vector<std::vector<double> > >& v);

    void read(
            const std::string& filename,
            const std::string& datasetName,
            double& res);

    void read(
            const H5::H5File& file,
            const std::string& datasetName,
            double& res);

    void read(
            const std::string& filename,
            const std::string& datasetName,
            std::vector<double>& res);

    void read(
            const H5::H5File& file,
            const std::string& datasetName,
            std::vector<double>& res);

    void read(
            const std::string& filename,
            const std::string& datasetName,
            std::vector<std::vector<double> >& res);

    void read(
            const H5::H5File& file,
            const std::string& datasetName,
            std::vector<std::vector<double> >& res);

    void read(
            const std::string& filename,
            const std::string& datasetName,
            std::vector<std::vector<std::vector<double> > > & res);

    void read(
            const H5::H5File& file,
            const std::string& datasetName,
            std::vector<std::vector<std::vector<double> > > & res);

    bool doesFileExists(const std::string& filename);

    /**
     * \brief creates an empty HDF5 file. It deletes the file if existing.
     * \param[in] filename
     */
    void createEmptyHdf5File(const std::string& filename);

    /**
     * \brief checks if a HDF5 file exists and if not, it creates an
     *        empty HDF5 file.
     * \param[in] filename
     */
    void ifFileDoesNotExistsCreateOne(const std::string& filename);

    /**
     * \brief creates and opens an empty. It deletes the file if existing.
     * \param[in] filename
     * \return Hdf5 file descriptor
     */
    H5::H5File openEmptyHdf5File(const std::string& filename);

    /**
     * \brief opens an existing HDF5 file or creates ones.
     * \param[in] filename
     * \return Hdf5 file descriptor
     */
    H5::H5File openOrCreateAHdf5File(const std::string& filename);
}

#endif
