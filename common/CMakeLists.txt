
find_package(quill REQUIRED)

add_library(unid_lib STATIC)

target_sources(unid_lib PRIVATE
  PRIVATE
    ./network/pool/libpool.c
)

target_sources (unid_lib PUBLIC
  ./include/unid_lib.h
)

target_include_directories(unid_lib PUBLIC
  ./include
)

target_link_libraries(unid_server
  PRIVATE 
    quill::quill
)

