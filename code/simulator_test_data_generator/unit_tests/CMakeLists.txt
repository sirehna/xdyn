# ------8<---[LINES TO MODIFY WHEN CHANGING MODULE]----->8-----
set(MODULE_UNDER_TEST test_data_generator)
project (${MODULE_UNDER_TEST}_tests)
FILE(GLOB SRC src/DataSourceTest.cpp
              src/SignalContainerTest.cpp
              src/TypeCoercionTest.cpp)
# ------8<---------------------------------------------->8-----

# Include directories
include_directories(SYSTEM inc)
