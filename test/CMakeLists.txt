
add_executable(unid_test)

target_sources(unid_test PRIVATE ../server/network/udp_server.cpp)

target_include_directories(unid_test PUBLIC include)

target_link_libraries(unid_test
  PRIVATE asio::asio Catch2::Catch2WithMain quill::quill
  PRIVATE common)

