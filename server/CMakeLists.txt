
find_package(asio REQUIRED)          # Header only
find_package(Microsoft.GSL REQUIRED) # Header only
find_package(quill REQUIRED)

add_executable(unid_server)

target_sources(unid_server
  PRIVATE
    ./src/main.cpp
    ./src/network/network.cpp
    ./src/network/udp_server.cpp
)

target_include_directories(unid_server
  PRIVATE
    ./include
)

target_link_libraries(unid_server
  PRIVATE 
    asio::asio
    Microsoft.GSL::GSL
    quill::quill
)

