add_executable(unid_client)

target_sources(
  unid_client
  PRIVATE main.cpp
          network/udp_client.cpp
)

target_include_directories(unid_client PRIVATE include)

target_link_libraries(
  unid_client
  PUBLIC asio::asio
         Microsoft.GSL::GSL
         quill::quill
         tomlplusplus::tomlplusplus
  PRIVATE common
)
