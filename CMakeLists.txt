cmake_minimum_required(VERSION 3.15)

# Set project name and language
project(OptimizeZswap CXX)

# Add the library and executable subdirectories
add_subdirectory(src)
add_subdirectory(apps)
add_subdirectory(tests)