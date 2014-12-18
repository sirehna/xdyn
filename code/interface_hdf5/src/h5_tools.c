#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "macrosId.h"
#include "h5_tools.h"

herr_t _h5_addRawStringsAsAnAttributeToADataSet(
        const hid_t                dset,
        char         const * const attributeName,
        const size_t               numberOfStringsToAdd,
        const size_t               maxLengthStringsToAdd,
        char const * const         rawStringsToWrite);

char * _h5_concatenatePaddedArrayOfStrings(
        size_t numberOfStrings,
        char * * strings,
        size_t * const maxLengthStringsToAdd);

/**
 * \param[in] fileName Name of the HDF5 file
 * \return Boolean indicating whether the file exists
 */
hbool_t h5_doesFileExists(char const * const fileName)
{
    FILE * const file = fopen(fileName, "rb");
    if(file != NULL)
    {
        fclose(file);
        return 1;
    }
    return 0;
}

/**
 * \brief opens a HDF5 file if file exists, or creates a new one
 * \param[in] fileName Name of the HDF5 file
 * \return idFile
 */
hid_t h5_getFileId(
        char const * const fileName)
{
    hid_t idFile = 0;
    if(h5_doesFileExists(fileName) == 1)
    {
        idFile = H5Fopen(fileName, H5F_ACC_RDWR, H5P_DEFAULT);
    }
    else
    {
        idFile = H5Fcreate(fileName, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    }
    return idFile;
}

/**
 * \brief writes a dataset in a HDF5 file
 * \param[in] file HDF5 file
 * \param[in] id HDF5 type of the dataset to write
 * \param[in] dataSetName Name of the dataSet to add to the HDF5 file
 * \param[in] ndims Number of dimensions of the dataset
 * \param[in] dims Dimensions of the dataset
 * \param[in] dataSet Dataset to write
 * \return status Error code
 * \note If the dataset is already present in the file, then it is replaced by
 *       the new one.
 */
herr_t h5_addDataSetAtRoot(
        const hid_t           file,
        const hid_t           id,
        char const * const    dataSetName,
        const int             ndims,
        hsize_t const * const dims,
        void const * const    dataSet)
{
    herr_t status = 0;
    hid_t space = 0;
    hid_t dset = 0;
    if(H5Lexists(file, dataSetName, H5P_DEFAULT)==1)
    {
        if(H5Ldelete(file, dataSetName, H5P_DEFAULT) < 0)
        {
            printf("%s : H5Ldelete failed\n",__FILE__);
        }
    }
    space  = H5Screate_simple(ndims, dims, NULL);
    dset   = H5Dcreate2(file, dataSetName, id, space, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Dwrite(dset, id, H5S_ALL, H5S_ALL, H5P_DEFAULT, dataSet);
    status = H5Dclose(dset);
    status = H5Sclose(space);
    return status;
}

herr_t h5_addAnArrayOfStringsAsAnAttributeToADataSet(
    hid_t                      dset,
    char         const * const attributeName,
    size_t                     numberOfStringsToAdd,
    char               * *     stringsToAdd)
{
    herr_t      status = 0;
    size_t      maxLengthStringsToAdd = 0;
    char        * m = NULL;
    if((attributeName==NULL) || (numberOfStringsToAdd==0) || (stringsToAdd==NULL))
    {
        return status;
    }
    m = _h5_concatenatePaddedArrayOfStrings(numberOfStringsToAdd, stringsToAdd, &maxLengthStringsToAdd);
    status = _h5_addRawStringsAsAnAttributeToADataSet(
                dset, attributeName, numberOfStringsToAdd, maxLengthStringsToAdd,m);
    free(m);
    return status;
}

/**
 * \brief Function used to concatenate a set of strings, based on the longest
 *        string
 * \param[in] numberOfStrings
 * \param[in] strings List of strings to concatenate
 * \param[out] maxLengthStrings
 * \return m Pointer to the newly allocated concatenated string
 */
char * _h5_concatenatePaddedArrayOfStrings(
        size_t numberOfStrings,
        char * * strings,
        size_t * const maxLengthStrings)
{
    char * m = NULL;
    size_t i =0, tmp = 0;
    size_t maxLength = 0;
    *maxLengthStrings = 0;
    if(numberOfStrings==0)
    {
        return m;
    }
    for(i=0;i<numberOfStrings;++i)
    {
        tmp = strlen(strings[i]);
        maxLength = (tmp>maxLength)?tmp:maxLength;
    }
    m = (char *) calloc((maxLength+1)*numberOfStrings,sizeof(char));
    for(i=0;i<numberOfStrings;++i)
    {
        strncpy(m+(i*(maxLength+1)),strings[i],strlen(strings[i]));
    }
    *maxLengthStrings = maxLength;
    return m;
}

/**
 * \brief add an attribute to a hdf5 dataset given as an array of padded strings
 * \param[in] dset Dataset
 * \param[in] attributeName Name to give to the attribute to add
 * \param[in] numberOfStringsToAdd Number of strings to add
 * \param[in] maxLengthStringsToAdd Maximu length of the strings to add
 * \param[in] rawStringsToWrite Concatenated padded strings to add
 * \return status HDF5 error code
 */
herr_t _h5_addRawStringsAsAnAttributeToADataSet(
        const hid_t                dset,
        char         const * const attributeName,
        const size_t               numberOfStringsToAdd,
        const size_t               maxLengthStringsToAdd,
        char const * const         rawStringsToWrite)
{
    herr_t      status = 0;
    hid_t       filetype = 0;
    hid_t       space = 0;
    hid_t       memtype = 0;
    hid_t       attr = 0;
    hsize_t     dims[1] = {0};
    dims[0] = numberOfStringsToAdd;
    filetype= H5Tcopy(H5T_FORTRAN_S1);
    status  = H5Tset_size(filetype, maxLengthStringsToAdd);
    memtype = H5Tcopy(H5T_C_S1);
    status  = H5Tset_size(memtype, maxLengthStringsToAdd+1);
    space   = H5Screate_simple(1, dims, NULL);
    attr    = H5Acreate(dset, attributeName, filetype, space, H5P_DEFAULT, H5P_DEFAULT);
    status  = H5Awrite(attr, memtype, rawStringsToWrite);
    status  = H5Aclose(attr);
    status  = H5Sclose(space);
    status  = H5Tclose(filetype);
    status  = H5Tclose(memtype);
    return status;
}

/**
 *  \brief displays the data type of a HDF5 element
 */
void h5_displayDataType(FILE *f, hid_t tid)
{
    const H5T_class_t t_class = H5Tget_class(tid);
    h5_displayDataTypeFromClass(f, t_class);
}

void h5_displayDataTypeFromClass(FILE * const f, H5T_class_t t_class)
{
    if(t_class < 0)
    {
        fprintf(f," Invalid datatype.\n");
    }
    else
    {
        if(t_class == H5T_INTEGER)
        {
            fprintf(f," Datatype is 'H5T_INTEGER'.\n");
        }
        else if(t_class == H5T_FLOAT)
        {
            fprintf(f," Datatype is 'H5T_FLOAT'.\n");
        }
        else if(t_class == H5T_STRING)
        {
            fprintf(f," Datatype is 'H5T_STRING'.\n");
        }
        else if(t_class == H5T_BITFIELD)
        {
            fprintf(f," Datatype is 'H5T_BITFIELD'.\n");
        }
        else if(t_class == H5T_OPAQUE)
        {
            fprintf(f," Datatype is 'H5T_OPAQUE'.\n");
        }
        else if(t_class == H5T_COMPOUND)
        {
            fprintf(f," Datatype is 'H5T_COMPOUND'.\n");
        }
        else if(t_class == H5T_ARRAY)
        {
            fprintf(f," Datatype is 'H5T_ARRAY'.\n");
        }
        else if(t_class == H5T_ENUM)
        {
            fprintf(f," Datatype is 'H5T_ENUM'.\n");
        }
        else
        {
            fprintf(f," Datatype is 'Other'.\n");
        }
    }
}

/**
 * \brief wrapping function of H5Tclose
 * \param type_id
 * \return Error code of function H5Tclose
 * \note Done so that one does not have to link hdf5, (especially for unit tests)
 */
herr_t h5_H5Tclose(hid_t type_id)
{
    return H5Tclose(type_id);
}
