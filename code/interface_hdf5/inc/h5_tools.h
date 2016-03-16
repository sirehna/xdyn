#ifndef H5_TOOLS_H
#define H5_TOOLS_H

#include <stddef.h>
#include <stdio.h>
#include "hdf5.h"

#ifdef __cplusplus
extern "C" {
#endif

hbool_t h5_doesFileExists(
        char const * const fileName);

hid_t h5_getFileId(
        char const * const fileName);

herr_t h5_addDataSetAtRoot(
        const hid_t           file,
        const hid_t           id,
        char const * const    dataSetName,
        const int             ndims,
        hsize_t const * const dims,
        void const * const    dataSet);

herr_t h5_addAnArrayOfStringsAsAnAttributeToADataSet(
    hid_t                      dset,
    char         const * const attributeName,
    size_t                     numberOfStringsToAdd,
    char               * *     stringsToAdd);

void h5_displayDataType(FILE *f,hid_t tid);

void h5_displayDataTypeFromClass(FILE *f, H5T_class_t t_class);

herr_t h5_H5Tclose(hid_t type_id);

#ifdef __cplusplus
}
#endif

#endif
