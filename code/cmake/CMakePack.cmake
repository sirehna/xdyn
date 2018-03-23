# Install the dependencies of the program
IF(MINGW)
    # Caution: No cross-compiling is taken into account:
    # If we use a 32 bit compiler, we will get a 32 executable.
    # The same for 64 bit platform
    IF(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        SET(PLATFORM_ABI x64)
    ELSE()
        SET(PLATFORM_ABI x86)
    ENDIF()
    # 2 Check the exception mechanism
    GET_FILENAME_COMPONENT(MINGW_BIN_PATH ${CMAKE_CXX_COMPILER} PATH)
    MESSAGE(STATUS "MINGW BIN PATH = ${MINGW_BIN_PATH}")
    SET(MINGW_LIB_DIRECTORY "")
    IF(${PLATFORM_ABI} MATCHES "x64")
        SET(MINGW_LIB_DIRECTORY "x86_64-w64-mingw32")
    ELSE()
        SET(MINGW_LIB_DIRECTORY "i686-w64-mingw32")
    ENDIF()
    IF(EXISTS "${MINGW_BIN_PATH}/../${MINGW_LIB_DIRECTORY}/")
        FILE(GLOB MINGW_DLLS ${MINGW_BIN_PATH}/../${MINGW_LIB_DIRECTORY}/lib/*.dll)
    ELSE()
        # Maybe Dlls are present in the mingw bin directory
        FILE(GLOB MINGW_DLLS ${MINGW_BIN_PATH}/*.dll)
    ENDIF()
    FOREACH(f ${MINGW_ADDITIONAL_LIB})
        IF(EXISTS ${f})
            MESSAGE(STATUS "MINGW DLL will be installed = ${f}")
            INSTALL(FILES ${f} DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
        ENDIF()
    ENDFOREACH()
ELSE()
    INSTALL(FILES /usr/lib/x86_64-linux-gnu/libgfortran.so.3.0.0 DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /usr/lib/x86_64-linux-gnu/libquadmath.so.0.0.0 DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.20 DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /lib/x86_64-linux-gnu/libm-2.19.so DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /lib/x86_64-linux-gnu/libgcc_s.so.1 DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /lib/x86_64-linux-gnu/libc-2.19.so DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /lib/x86_64-linux-gnu/libpthread-2.19.so DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /lib/x86_64-linux-gnu/libdl-2.19.so DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
    INSTALL(FILES /lib/x86_64-linux-gnu/ld-2.19.so DESTINATION ${LIBRARY_OUTPUT_DIRECTORY})
ENDIF()

#####################################"
# Instructions to build an installer
IF(UNIX OR MSYS)
    INCLUDE(InstallRequiredSystemLibraries)
    SET(CPACK_PACKAGE_NAME "xdyn")
    SET(CPACK_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION OFF)
    SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "X-DYN : Simulateur 6ddl de navires")
    SET(CPACK_PACKAGE_VENDOR "DCNS Research")
    #SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/ReadMe.txt")
    SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_BINARY_DIR}/Copyright.txt")
    SET(CPACK_PACKAGE_VERSION_MAJOR ${${PROJECT_NAME}_MAJOR_VERSION})
    SET(CPACK_PACKAGE_VERSION_MINOR ${${PROJECT_NAME}_MINOR_VERSION})
    SET(CPACK_PACKAGE_VERSION_PATCH 0)
    SET(CPACK_PACKAGE_INSTALL_DIRECTORY "X-DYN")
    SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
    SET(CPACK_COMPONENT_INCLUDE_TOPLEVEL_DIRECTORY OFF)
    #SET(CPACK_STRIP_FILES ON)
    IF(WIN32)
        # There is a bug in NSIS that does not handle full unix paths properly. Make
        # sure there is at least one set of four (4) backlasshes.
        #SET(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/Utilities/Release\\\\InstallIcon.bmp")
        SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
        SET(CPACK_NSIS_INSTALLED_ICON_NAME "X-DYN")
        SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
        SET(CPACK_NSIS_HELP_LINK "http:\\\\\\\\www.sirehna.com")
        SET(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\www.sirehna.com")
        SET(CPACK_NSIS_CONTACT "charles-edouard.cady@sirehna.com")
        SET(CPACK_NSIS_MODIFY_PATH ON)
        #SET(CPACK_PACKAGE_EXECUTABLES "" "")
    ELSE(WIN32)
        SET(CPACK_STRIP_FILES "./x-dyn-installer")
        SET(CPACK_SOURCE_STRIP_FILES "")
        SET(CPACK_PACKAGE_EXECUTABLES "" "")
        SET(CPACK_GENERATOR "DEB")
        SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Charles-Edouard Cady")
        ##CPACK_PACKAGING_INSTALL_PREFIX : Sets the default root that the generated package installs into, '/usr' is the default for the debian and redhat generators
        #SET(CPACK_GENERATOR "RPM")
        SET(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}")
        ##To generate a RPM file, one needs program rpmbuild that can be
        ##installed with
        ##su
        ##yum install rpm-build
        ##yum install --skip-broken rpm-build
        ##To see the content of a RPM file, use the following command
        ##rpm -q -filesbypkg -p DYSCOASM-0.0.0-Linux.rpm
        ##
        ##rpm -i --prefix=/home/people/jacquenot/simulateurIrtJv/ simulateurIrtJv_installer_1589M.rpm
    ENDIF(WIN32)
    INCLUDE(CPack)
ENDIF()
