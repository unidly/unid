
find_package(asio REQUIRED)          # Header only
find_package(Catch2 REQUIRED)

add_executable(unid_test)

target_sources(unid_test PRIVATE
  ../server/src/network/udp_server.cpp
  network_udp_server.cpp
)

target_link_libraries(unid_test
  PRIVATE 
    asio::asio
    Catch2::Catch2WithMain
)

