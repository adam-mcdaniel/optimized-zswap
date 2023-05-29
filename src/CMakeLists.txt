# Create the library target
add_library(optimize-zswap STATIC allocator.cpp)

# Set include directories if needed
target_include_directories(optimize-zswap PUBLIC include)
target_include_directories(optimize-zswap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(optimize-zswap PUBLIC ${Boost_LIBRARIES})
# target_sources(optimize-zswap PUBLIC ${KERNELHEADERS_DIR}/include/linux/zsmalloc.h)
