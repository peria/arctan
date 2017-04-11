cmake_minimum_required(VERSION 2.8)

arctan_add_library(drm
  drm.cc drm2.cc drm3.cc drm4.cc drm5.cc drm6.cc)
# drm4_test

arctan_add_executable(drm_exe drm_main.cc)
target_link_libraries(drm_exe arctan_drm arctan_number)