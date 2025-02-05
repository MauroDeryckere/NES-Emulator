# @REGION PROJECT SETUP

cmake_minimum_required(VERSION 3.27)

# Any output by targets -> binary dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# prevent installing to system directories. 
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE INTERNAL "")

project(NES_EMULATOR)

# Set the name of the executable
set(EXECUTABLE_NAME ${PROJECT_NAME})

if(MSVC)
    if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
        add_definitions(/MP) # parallelize each target, unless Ninja is the generator
    endif()
endif()

if (APPLE)
    # Disable shared builds on platforms where it does not make sense to use them
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
endif()

# @ENDREGION PROJECT SETUP


# @REGION SOURCE FILES & LIBRARIES

# Sources
add_subdirectory(src EXCLUDE_FROM_ALL)

add_executable(${EXECUTABLE_NAME} ${SOURCES})

target_precompile_headers(${EXECUTABLE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Emulator/emulator_pch.h)


# Dependencies
add_subdirectory(3rdParty EXCLUDE_FROM_ALL)
# Link the 3rd party interface library to the project
target_link_libraries(${PROJECT_NAME} PRIVATE 3RDPARTY)
# Set cpp 23 standard
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# @ENDREGION SOURCE FILES & LIBRARIES


# @REGION PLATFORMS

include(CheckCXXCompilerFlag)

# Check for -Wdocumentation flag
check_cxx_compiler_flag("-Wdocumentation" HAVE_GCC_WDOCUMENTATION)
if(HAVE_GCC_WDOCUMENTATION)
  add_compile_options(-Wdocumentation)
endif()

# Check for -Wdocumentation-unknown-command flag
check_cxx_compiler_flag("-Wdocumentation-unknown-command" HAVE_GCC_WDOCUMENTATION_UNKNOWN_COMMAND)
if(HAVE_GCC_WDOCUMENTATION_UNKNOWN_COMMAND)
  add_compile_options(-Wdocumentation-unknown-command)
endif()

# Check for -Wcomment-block-commands flag
check_cxx_compiler_flag("-Wcomment-block-commands" HAVE_GCC_COMMENT_BLOCK_COMMANDS)
if(HAVE_GCC_COMMENT_BLOCK_COMMANDS)
  add_compile_options(-Wcomment-block-commands)
endif()

# Dealing with assets
# We have some non-code resources that our application needs in order to work. How we deal with those differs per platform.
if (APPLE)
    # on Apple targets, the application bundle has a "resources" subfolder where we can place our assets.
    # SDL_GetBasePath() gives us easy access to that location.
    set(input_root "${CMAKE_CURRENT_LIST_DIR}/src")
    macro(add_resource FILE)
        file(RELATIVE_PATH relpath "${input_root}" "${FILE}")
        get_filename_component(relpath "${relpath}" DIRECTORY)
        target_sources(${EXECUTABLE_NAME} PRIVATE ${FILE})
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${relpath}")
    endmacro()
endif()

# Copy resources after the build process
if(WIN32)
    # Ensure that the Resources directory is created inside the final output directory
    add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/Resources"
            "$<TARGET_FILE_DIR:${EXECUTABLE_NAME}>/Resources"
    )
endif()

# set some extra configs for each platform
set_target_properties(${EXECUTABLE_NAME} PROPERTIES 
    # On macOS, make a proper .app bundle instead of a bare executable
    MACOSX_BUNDLE TRUE

    # Currently not building on mobile but keep this in, in case we do in the future

    # Set the Info.plist file for Apple Mobile platforms. Without this file, your app
    # will not launch. 
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in"

    # in Xcode, create a Scheme in the schemes dropdown for the app.
    XCODE_GENERATE_SCHEME TRUE
    # Identification for Xcode
    XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "NES_EMULATOR"
	XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "NES_EMULATOR"
	XCODE_ATTRIBUTE_CURRENTYEAR "${CURRENTYEAR}"
)

# On macOS Platforms, ensure that the bundle is valid for distribution by calling fixup_bundle.
# note that fixup_bundle does not work on iOS, so you will want to use static libraries 
# or manually copy dylibs and set rpaths
if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
    # tell Install about the target, otherwise fixup won't know about the transitive dependencies
    install(TARGETS ${EXECUTABLE_NAME}
    	BUNDLE DESTINATION ./install COMPONENT Runtime
   	    RUNTIME DESTINATION ./install/bin COMPONENT Runtime
    )
	
    set(DEP_DIR "${CMAKE_BINARY_DIR}")  # where to look for dependencies when fixing up
    INSTALL(CODE 
        "include(BundleUtilities)
        fixup_bundle(\"$<TARGET_BUNDLE_DIR:${EXECUTABLE_NAME}>\" \"\" \"${DEP_DIR}\")
        " 
    )
    set(CPACK_GENERATOR "DragNDrop")
    include(CPack)
endif()

# @ENDREGION PLATFORMS

# on Visual Studio, set our app as the default project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT "${EXECUTABLE_NAME}")

# Include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/Engine")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/Emulator")
