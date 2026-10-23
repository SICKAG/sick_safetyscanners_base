include(CMakeFindDependencyMacro)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost COMPONENTS chrono thread REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/sick_safetyscanners_baseTargets.cmake")