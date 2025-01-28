# Add sub directories for any third party libraries
# This ensures any cmake lists in these files are also added to the project

# Any specifc settings for SDL: 
# Subsystems:  
set(SDL_HAPTIC OFF)
set(SDL_CAMERA OFF)
set(SDL_SENSOR OFF)
set(SDL_POWER OFF) # May be worth enabling if we do release on mobile to save battery

# Dummy
set(SDL_DUMMYAUDIO  OFF)
set(SDL_DUMMYVIDEO  OFF)

# Rendering
set(SDL_OPENGL: OFF)
set(SDL_OPENGLES: OFF) # May be required if we do release on mobile

# Tests & similar
set(DSDL_TESTS OFF)
set(DSDL_TEST_LIBRARY OFF)
set(DSDL_DISABLE_INSTALL_DOCS OFF)
set(DSDL_INSTALL_TESTS OFF)

set(SDL_TESTS OFF)
set(SDL_TEST_LIBRARY OFF)
set(SDL_DISABLE_INSTALL_DOCS OFF)
set(SDL_INSTALL_DOCS OFF)
set(SDL_INSTALL_TESTS OFF)

set(SDL_EXAMPLES OFF)

# We don't want to build shared libraries
set(SDL_SHARED OFF)
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
