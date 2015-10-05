project(mpialgebra)
cmake_minimum_required(VERSION 2.8)

find_package(HDF5 REQUIRED COMPONENTS CXX)
find_package(MPI REQUIRED)

set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
include_directories(${HDF5_INCLUDE_DIRS})

ADD_EXECUTABLE(main main.cxx mpialgebra.cxx)
TARGET_LINK_LIBRARIES(main ${HDF5_LIBRARIES} ${HDF5_CXX_LIBRARIES})
