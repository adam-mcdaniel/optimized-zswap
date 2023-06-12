# Get all the source files in the apps directory
file(GLOB APP_SOURCES *.cpp)

# Iterate over each source file and create an executable
foreach(APP_SOURCE ${APP_SOURCES})
    # Get the file name without extension
    get_filename_component(APP_NAME ${APP_SOURCE} NAME_WE)
    # Create the executable target
    add_executable(${APP_NAME} ${APP_SOURCE})

    target_include_directories(${APP_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    # Link the executable with the library (if applicable)
    target_link_libraries(${APP_NAME} PUBLIC ${PROJECT_NAME})
    # target_link_libraries(${APP_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/src/lib${PROJECT_NAME}.a)
endforeach()
