add_library(
  common
  STATIC
  "src/quill.cpp"
  "src/libpool.c"
  "src/mempool.cpp"
  "src/config.cpp"
  "src/utility.cpp"
)

# Source file based compiler options and definitions
set_source_files_properties("src/config.cpp" PROPERTIES COMPILE_OPTIONS "-w")
set_source_files_properties("src/libpool.cpp" PROPERTIES COMPILE_DEFINITIONS "LIBPOOL_NO_VALGRIND")

# Publically accessible include files
target_include_directories(
  common
  PUBLIC include
  PRIVATE "src/"
)

# Link to external libraries used in common library
target_link_libraries(
  common
  PUBLIC asio::asio
         Microsoft.GSL::GSL
         quill::quill
         tomlplusplus::tomlplusplus
)

get_target_property(MY_INCLUDES common INCLUDE_DIRECTORIES)
message(STATUS "Common library Search paths: ${MY_INCLUDES}")
