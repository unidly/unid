add_library(libpool_lib STATIC libpool.c libpool.h)

# Specify the public include directories for targets that link to this library
target_include_directories(libpool_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

