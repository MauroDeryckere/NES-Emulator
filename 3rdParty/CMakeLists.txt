# Add sub directories for any third party libraries
# This ensures any cmake lists in these files are also added to the project

# Any specifc settings for SDL: 
#Sybsystems:  
set(SDL_HAPTIC OFF)
set(SDL_CAMERA OFF)
set(SDL_SENSOR OFF)
set(SDL_POWER OFF) # May be worth enabling if we do release on mobile to save battery

set(SDL_DUMMYAUDIO  OFF)
set(SDL_DUMMYVIDEO  OFF)

set(DSDL_TESTS OFF)
set(DSDL_TEST_LIBRARY OFF)
set(DSDL_DISABLE_INSTALL_DOCS OFF)
set(DSDL_INSTALL_TESTS OFF)

set(SDL_SHARED OFF) # We want to statically link SDL
set(SDL_STATIC ON)

# We use EXCLUDE_FROM_ALL so that its install targets and configs don't
# pollute upwards into our configuration.
add_subdirectory(SDL EXCLUDE_FROM_ALL)	

# Create an interface library for all third party libraries
add_library(3RDPARTY INTERFACE)
# Link with SDL
target_link_libraries(3RDPARTY INTERFACE 
					  SDL3::SDL3
)
