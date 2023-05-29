cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# Set project name and language
project(OptimizeZswap CXX)
set(CMAKE_CXX_STANDARD 20)

find_package(KernelHeaders REQUIRED)
if (NOT KERNELHEADERS_DIR)
    message(FATAL_ERROR "Kernel headers were not found")
endif ()
message(STATUS "Kernel source directory: ${KERNELHEADERS_INCLUDE_DIRS}")
# include_directories(${KERNELHEADERS_DIR}/include)

# target_sources(optimize-zswap PUBLIC ${KERNELHEADERS_INCLUDE_DIRS}/linux/zsmalloc.h)

find_package(Boost 1.78 REQUIRED COMPONENTS system)
set(Boost_COMPONENTS system)

include(DownloadProject.cmake)
download_project(PROJ           pfr_non_boost
                 GIT_REPOSITORY https://github.com/apolukhin/pfr_non_boost
                 GIT_TAG        master
)
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