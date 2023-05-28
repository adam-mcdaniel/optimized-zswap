cmake_minimum_required(VERSION 3.26)

# Set project name and language
project(OptimizeZswap CXX)
set(CMAKE_CXX_STANDARD 23)

find_package(Boost 1.78 REQUIRED COMPONENTS system)
set(Boost_COMPONENTS system)

include(DownloadProject.cmake)
download_project(PROJ           pfr_non_boost
                 GIT_REPOSITORY https://github.com/apolukhin/pfr_non_boost
                 GIT_TAG        master
)
include_directories(${pfr_non_boost_SOURCE_DIR}/include)


# Add the Boost include directories and libraries
include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})
link_directories(${LIBRARY_OUTPUT_DIRECTORY})

# Add the library and executable subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(apps)