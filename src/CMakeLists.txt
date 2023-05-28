# Create the library target
add_library(OptimizeZswapLib STATIC allocator.cpp)

# Set include directories if needed
target_include_directories(OptimizeZswapLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
