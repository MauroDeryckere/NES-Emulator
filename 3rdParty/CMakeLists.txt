# Add sub directories for any third party libraries
# This ensures any cmake lists in these files are also added to the project

# Any specifc settings for SDL: 
add_subdirectory(SDL EXCLUDE_FROM_ALL)	

# Create an interface library for all third party libraries
add_library(3RDPARTY INTERFACE)
# Link with SDL
target_link_libraries(3RDPARTY INTERFACE 
					  SDL3::SDL3
)
