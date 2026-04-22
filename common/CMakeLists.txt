add_library(
  common
  STATIC
  "src/quill.cpp"
  "src/libpool.c"
  "src/mempool.cpp"
)

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
)
