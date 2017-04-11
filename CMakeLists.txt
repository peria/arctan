project(arctan)

# ----------------------------------------------------------------------
# cmake settings

cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

enable_testing()

# ----------------------------------------------------------------------
# Check packages, libraries, etc.

find_package(PkgConfig)
find_library(LIB_GFLAGS gflags)
pkg_search_module(GLOG REQUIRED libglog)

# ----------------------------------------------------------------------
# Set include directories, c++ options, etc.

function(ppi_add_cxx_flags flags)
  set(CMAKE_CXX_FLAGS "${flags} ${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
endfunction()

ppi_add_cxx_flags("-std=c++11")
ppi_add_cxx_flags("-Wall -Wextra")
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  ppi_add_cxx_flags("-O0")
  ppi_add_cxx_flags("-g")
else()
  ppi_add_cxx_flags("-O2")
endif()
ppi_add_cxx_flags("-march=native")
ppi_add_cxx_flags("-DSRC_DIR=\"${CMAKE_SOURCE_DIR}\"")

include_directories(.)
include_directories(${GMP_INCLUDE_DIRS})
include_directories(${GLOG_INCLUDE_DIRS})

link_directories(${GMP_LIB_DIRS})

# ----------------------------------------------------------------------
# Add subdirectories

function(arctan_add_library target)
  add_library(arctan_${target} ${ARGN})
  target_link_libraries(arctan_${target} arctan_base)
endfunction()

function(arctan_target_link_libraries target)
  target_link_libraries(${target} ${LIB_GFLAGS})
  target_link_libraries(${target} ${GLOG_LIBRARIES})
  target_link_libraries(${target} ${LIB_GMP})
  target_link_libraries(${target} ${ARGN})
endfunction()

function(arctan_add_executable target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} arctan_base)
  arctan_target_link_libraries(${target})
endfunction()

add_subdirectory(base)
add_subdirectory(drm)
add_subdirectory(number)
add_subdirectory(search)

# ----------------------------------------------------------------------
# Display settings
message(STATUS "Build type:" ${CMAKE_BUILD_TYPE})