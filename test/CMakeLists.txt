add_executable(unid_test)

target_sources(
  unid_test
  PRIVATE ../server/network/udp_server.cpp
          ../client/network/udp_client.cpp
          ../common/src/mempool.cpp
          ../common/src/config.cpp
          server_network_udp_test.cpp
          client_network_udp_test.cpp
          common_mempool_test.cpp
          common_config_test.cpp
)

set_source_files_properties("../common/src/config.cpp" PROPERTIES COMPILE_OPTIONS "-w")

target_include_directories(unid_test PUBLIC include)

target_link_libraries(
  unid_test
  PRIVATE asio::asio
          Catch2::Catch2WithMain
          quill::quill
          tomlplusplus::tomlplusplus
  PRIVATE common
)
