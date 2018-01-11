add_library(common STATIC src/common_functions.cc)
#include_directories(.)
target_include_directories(common PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
