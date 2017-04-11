cmake_minimum_required(VERSION 2.8)

arctan_add_library(search search.cc)

arctan_add_executable(search_exe search_main.cc)
target_link_libraries(search_exe arctan_search)
