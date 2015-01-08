#include "macrosId.h"
#include "h5_tools.h"
#include "h5_version.h"

#ifndef STR
#define STR(s) STR_(s)
#endif

#ifndef STR_
#define STR_(s) #s
#endif

#if defined(_WIN32) && defined(_MSC_VER)
void h5_writeFileDescriptionMSVC(char * const s);
void h5_writeFileDescriptionMSVC(char * const s)
{
#if (_MSC_VER == 1900)
    sprintf(s, "             MSVC Version           : 1900 -- MSVC++ 13.0 (Visual Studio 2014)\n");
#elif (_MSC_VER == 1800)
    sprintf(s, "             MSVC Version           : 1800 -- MSVC++ 12.0 (Visual Studio 2013)\n");
#elif (_MSC_VER == 1700)
    sprintf(s, "             MSVC Version           : 1700 -- MSVC++ 11.0 (Visual Studio 2012)\n");
#elif (_MSC_VER == 1600)
    sprintf(s, "             MSVC Version           : 1600 -- MSVC++ 10.0 (Visual Studio 2010)\n");
#elif (_MSC_VER == 1500)
    sprintf(s, "             MSVC Version           : 1500 -- MSVC++ 9.0  (Visual Studio 2008)\n");
#elif (_MSC_VER == 1400)
    sprintf(s, "             MSVC Version           : 1400 -- MSVC++ 8.0  (Visual Studio 2005)\n");
#elif (_MSC_VER == 1310)
    sprintf(s, "             MSVC Version           : 1310 -- MSVC++ 7.1 (Visual Studio 2003)\n");
#elif (_MSC_VER == 1300)
    sprintf(s, "             MSVC Version           : 1300 -- MSVC++ 7.0\n");
#elif (_MSC_VER == 1200)
    sprintf(s, "             MSVC Version           : 1200 -- MSVC++ 6.0\n");
#elif (_MSC_VER == 1100)
    sprintf(s, "             MSVC Version           : 1100 -- MSVC++ 5.0\n");
#else
    sprintf(s, "             MSVC Version           : %d\n", _MSC_VER);
#endif
}
#endif

/**
 * \brief adds a description to a HDF5 file
 * Below is a Python example of how to use this description
 * \code{.py}
import h5py
r=h5py.File('writeFileDescription.h5','r')
description = attrs[attrs.keys()[0]]
for i in range(len(description)):
    print(description[i])
 \endcode
 */
#define NUMBER_OF_ATTRIBUTES 5
#define MAXIMUM_ATTRIBUTE_LENGTH 255
int h5_writeFileDescription(
        char const * const fileName)
{
    herr_t ret = 0;
    hid_t file = h5_getFileId(fileName);
    h5_writeFileDescriptionGivenAFileId(file);
    ret = H5Fclose(file);
    return (ret<0)?0:1;
}

int h5_writeFileDescriptionGivenAFileId(hid_t file)
{
    hid_t root = 0, dataspace = 0, att = 0;
    hid_t filetype = 0;
    hid_t type = 0;
    herr_t ret;
    char string_att[NUMBER_OF_ATTRIBUTES][MAXIMUM_ATTRIBUTE_LENGTH+1] = {{0},{0},{0},{0},{0}};
    hsize_t dims[1] = {NUMBER_OF_ATTRIBUTES};

    sprintf(string_att[0], "%s", STR(PROJECT_NAME));
    sprintf(string_att[1], "Version : %s", STR(GIT_VERSION_SHORT));
    sprintf(string_att[2], "Compilation date : %s -- %s", __DATE__, __TIME__);
    sprintf(string_att[3], "Build type : %s -- %s -- %s\n", STR(TARGET_OS), STR(BUILD_TYPE), STR(TARGET_ARCH));
    #if defined(_WIN32) && defined(_MSC_VER)
    h5_writeFileDescriptionMSVC(string_att[4]);
    #elif __GNUC__
    sprintf(string_att[4], "GNU GCC Version : %d.%d.%d", __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__);
    #else
    sprintf(string_att[4], "Unconventional compiler");
    #endif

    root        = H5Gopen(file, "/", H5P_DEFAULT);
    filetype    = H5Tcopy(H5T_FORTRAN_S1);
    ret         = H5Tset_size(filetype, MAXIMUM_ATTRIBUTE_LENGTH+1);
    type        = H5Tcopy(H5T_C_S1);
    ret         = H5Tset_size(type, MAXIMUM_ATTRIBUTE_LENGTH+1);
    dataspace   = H5Screate_simple(1, dims, dims);
    att         = H5Acreate(root, "Description", filetype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
    ret         = H5Awrite(att, type, string_att);
    ret         = H5Aclose(att);
    ret         = H5Gclose(root);
    ret         = H5Tclose(type);
    ret         = H5Sclose(dataspace);
    return (ret<0)?0:1;
}
