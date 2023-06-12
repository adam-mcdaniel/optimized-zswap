# Create the library target
add_library(optimize-zswap STATIC allocator.cpp)

# Set include directories if needed
target_include_directories(optimize-zswap PUBLIC include)
target_include_directories(optimize-zswap PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(optimize-zswap PUBLIC ${Boost_LIBRARIES})
# target_sources(optimize-zswap PUBLIC ${KERNELHEADERS_DIR}/include/linux/zsmalloc.h)

find_package(ZLIB REQUIRED)
target_include_directories(optimize-zswap PUBLIC ${ZLIB_INCLUDE_DIRS})
target_link_libraries(optimize-zswap PUBLIC ${ZLIB_LIBRARIES})


add_library(instrument SHARED intel_pin_instrument.cpp)
target_include_directories(instrument PUBLIC include)
target_include_directories(instrument PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(PIN_ROOT /home/adam/Documentos/c++/optimized-zswap/pin-intel-linux)
# Add the include directory for Pin

# Print PIN_ROOT
message(STATUS "PIN_ROOT: ${PIN_ROOT}")
target_include_directories(instrument PUBLIC ${PIN_ROOT}/source/include/pin/)
target_include_directories(instrument PUBLIC ${PIN_ROOT}/source/include/pin/gen)
target_include_directories(instrument PUBLIC ${PIN_ROOT}/extras/components/include)
target_include_directories(instrument PUBLIC ${PIN_ROOT}/extras/crt/include)
target_include_directories(instrument PUBLIC ${PIN_ROOT}/extras/xed-intel64/include/xed)
target_link_libraries(instrument PUBLIC ${Boost_LIBRARIES})
target_link_libraries(instrument PUBLIC ${ZLIB_LIBRARIES})
target_link_libraries(instrument PUBLIC ${PIN_ROOT}/intel64/libpin.a)
