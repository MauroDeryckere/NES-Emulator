cmake_minimum_required(VERSION 3.27)
project(NES_EMULATOR)

# Any output by targets -> binary dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Sources
add_subdirectory(src)
# Create main executable and link to sources
add_executable(${PROJECT_NAME} ${SOURCES})

# Dependencies
add_subdirectory(3rdParty)
# Link the 3rd party interface library to the project
target_link_libraries(${PROJECT_NAME} PRIVATE 3RDPARTY)

# Enable all warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
# Set cpp 23 standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# Set the install directory differently based on the platform
if(APPLE)
    set(INSTALL_PREFIX "/Applications/myapp")
elseif(WIN32)
    set(INSTALL_PREFIX "C:/Program Files/myapp")
else()
    set(INSTALL_PREFIX "/usr/local/myapp")
endif()


# Install rules - Apple
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin  # Install the executable
    LIBRARY DESTINATION lib  # For shared libraries, if you have any
    ARCHIVE DESTINATION lib  # For static libraries, if you have any
)
