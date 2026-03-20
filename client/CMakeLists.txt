
find_package(asio REQUIRED)          # Header only
find_package(Microsoft.GSL REQUIRED) # Header only
find_package(quill REQUIRED)

add_executable(unid_client)

target_sources(unid_client
  PRIVATE
    main.cpp
    network/udp_client.cpp
)

target_include_directories(unid_server
  PRIVATE
    include
)

target_link_libraries(unid_server
  PRIVATE 
    asio::asio
    Microsoft.GSL::GSL
    quill::quill
)

