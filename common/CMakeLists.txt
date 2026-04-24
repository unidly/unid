add_library(
  common
  STATIC
  "src/quill.cpp"
  "src/libpool.c"
  "src/mempool.cpp"
  "src/config.cpp"
)

set_source_files_properties("src/config.cpp" PROPERTIES COMPILE_OPTIONS "-w")

# Specific compiler flags for building the common library
#target_compile_definitions(
#  common
#  PRIVATE LIBPOOL_NO_VALGRIND
#)

# Publically accessible include files
target_include_directories(
  common
  PUBLIC include
  PRIVATE "src/"
)

# Link to third - party libraries used in common_lib
target_link_libraries(
  common
  PUBLIC asio::asio
         Microsoft.GSL::GSL
         quill::quill
         tomlplusplus::tomlplusplus
)
