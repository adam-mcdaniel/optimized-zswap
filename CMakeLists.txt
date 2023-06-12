cmake_minimum_required(VERSION 3.0)

# Set project name and language
project(optimize-zswap C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
# Set the C and C++ compilers
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Specify the directory containing the CMake files
set(CMAKE_MODULES_DIR "${CMAKE_SOURCE_DIR}/cmake")
# Create a list of all CMake files in the directory
file(GLOB CMAKE_MODULES_LIST "${CMAKE_MODULES_DIR}/*.cmake")
# Include each CMake file from the list
foreach(CMAKE_MODULE_FILE ${CMAKE_MODULES_LIST})
  include(${CMAKE_MODULE_FILE})
endforeach()


# find_package(KernelHeaders REQUIRED)
if (NOT KERNELHEADERS_DIR)
    message(FATAL_ERROR "Kernel headers were not found")
endif ()
message(STATUS "Kernel source directory: ${KERNELHEADERS_INCLUDE_DIRS}")
# include_directories(${KERNELHEADERS_DIR}/include)

message(STATUS "KBUILD_CMD: ${KBUILD_CMD}")
execute_process(COMMAND make -C ${KERNELHEADERS_DIR} M=${PROJECT_SOURCE_DIR}/linux/mm RESULT_VARIABLE KBUILD_RESULT)
if (NOT KBUILD_RESULT EQUAL 0)
    message(FATAL_ERROR "KBUILD failed")
endif ()
# target_sources(optimize-zswap PUBLIC ${KERNELHEADERS_INCLUDE_DIRS}/linux/zsmalloc.h)

find_package(Boost 1.78 REQUIRED COMPONENTS system)
set(Boost_COMPONENTS system)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Boost was not found")
else ()
    message(STATUS "Boost include directory: ${Boost_INCLUDE_DIRS}")
endif ()

# include(DownloadProject.cmake)
download_project(PROJ           pfr_non_boost
                 GIT_REPOSITORY https://github.com/apolukhin/pfr_non_boost
                 GIT_TAG        master
)
if (NOT pfr_non_boost_SOURCE_DIR)
    message(FATAL_ERROR "PFR was not found")
else ()
    message(STATUS "PFR include directory: ${pfr_non_boost_SOURCE_DIR}/include")
endif ()
include_directories(${pfr_non_boost_SOURCE_DIR}/include)



# Use the KERNEL_HEADERS_ROOT variable to access the kernel source directory
message(STATUS "Kernel source directory: ${KERNEL_HEADERS_ROOT}")


# Add the Boost include directories and libraries
include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${LIBRARY_OUTPUT_DIRECTORY})

# Add the library and executable subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(apps)