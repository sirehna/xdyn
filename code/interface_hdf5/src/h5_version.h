#ifndef H5_VERSION_H
#define H5_VERSION_H

#include "hdf5.h"

#ifdef __cplusplus
extern "C" {
#endif

int h5_writeFileDescription(
        char const * const fileName);

int h5_writeFileDescriptionGivenAFileId(
        hid_t file);

#ifdef __cplusplus
}
#endif

#endif
