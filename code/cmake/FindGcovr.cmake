# - Find gcovr script
# Will define:
#
# GCOVR_EXECUTABLE - the gcovr script
#
# Uses:
#
# GCOVR_ROOT - root to search for the script
# ENV{GCOVR_ROOT} - root to search for the script

INCLUDE(FindPackageHandleStandardArgs)

FIND_PROGRAM(GCOVR_EXECUTABLE gcovr HINTS ${GCOVR_ROOT} "${GCOVR_ROOT}/bin" $ENV{GCOVR_ROOT} "$ENV{GCOVR_ROOT}/bin")

FIND_PACKAGE_HANDLE_STANDARD_ARGS(gcovr DEFAULT_MSG GCOVR_EXECUTABLE)

# only visible in advanced view
MARK_AS_ADVANCED(GCOVR_EXECUTABLE)
