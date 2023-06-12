# Create the library target
add_library(optimize-zswap STATIC ${CMAKE_CURRENT_SOURCE_DIR}/allocator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/zsmalloc.c ${KERNELHEADERS_DIR}/include/linux/zsmalloc.h)

# Set include directories if needed
target_include_directories(optimize-zswap PUBLIC include)
target_include_directories(optimize-zswap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# target_include_directories(optimize-zswap PUBLIC ${KERNELHEADERS_DIR}/include)
# target_include_directories(optimize-zswap PUBLIC ${KERNELHEADERS_DIR}/arch/x86/include)
target_include_directories(optimize-zswap PUBLIC /usr/include)
target_link_libraries(optimize-zswap PUBLIC ${Boost_LIBRARIES})

target_sources(optimize-zswap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/allocator.cpp ${CMAKE_CURRENT_SOURCE_DIR}/zsmalloc.c ${KERNELHEADERS_DIR}/include/linux/zsmalloc.h ${KERNELHEADERS_DIR}/include/linux/zsmalloc.h ${PROJECT_SOURCE_DIR}/zsmalloc.c)