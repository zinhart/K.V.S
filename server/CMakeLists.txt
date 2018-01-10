add_executable(server src/server.cc)
target_include_directories(server PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include") 
target_link_libraries(server common)

