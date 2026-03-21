find_package(asio REQUIRED)          # Header only
find_package(Microsoft.GSL REQUIRED) # Header only
find_package(quill REQUIRED)

add_subdirectory(logger)

add_executable(unid_client)

target_sources(unid_client
  PRIVATE
    main.cpp
    network/udp_client.cpp
)

target_include_directories(unid_client
  PRIVATE
    include
)

target_link_libraries(unid_client
  PRIVATE 
    asio::asio
    Microsoft.GSL::GSL
    quill::quill
    quill_client_lib
)
