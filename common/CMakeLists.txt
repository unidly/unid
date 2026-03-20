
find_package(asio REQUIRED)          # Header only
find_package(Microsoft.GSL REQUIRED) # Header only
find_package(quill REQUIRED)

add_library(unid_lib STATIC)

target_sources(unid_lib PRIVATE
  PRIVATE
    ./network/network.cpp
    ./network/udp_server.cpp
)

target_sources (unid_lib PUBLIC
  ./include/unid_lib.h
)

target_include_directories(unid_lib PUBLIC
  ./include
)

target_link_libraries(unid_server
  PRIVATE 
    asio::asio
    Microsoft.GSL::GSL
    quill::quill
)

