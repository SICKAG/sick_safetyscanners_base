include(CMakeFindDependencyMacro)
#find_dependency(Threads)
# Note that find_dependency does not support COMPONENTS or MODULE until 3.8.0
find_package(Boost COMPONENTS system thread REQUIRED)
include("${CMAKE_CURRENT_LIST_DIR}/sick_safetyscanners_baseTargets.cmake")

