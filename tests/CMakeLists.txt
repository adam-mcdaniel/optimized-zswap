# Get all the source files in the tests directory
file(GLOB TEST_SOURCES *.cpp)

# Iterate over each source file and create a test executable
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get the file name without extension
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    # Create the test executable target
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    # Link the test executable with the library (if applicable)
    target_link_libraries(${TEST_NAME} OptimizeZswapLib)
    # Register the test
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()
