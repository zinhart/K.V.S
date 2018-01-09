add_library(common STATIC src/common_functions.cc)
target_include_directories(common PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
