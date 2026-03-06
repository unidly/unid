/**
 * @file
 * @brief Unit tests for the UDP server class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../server/src/network/udp_server.hpp"
#include <asio.hpp>
#include <iostream>
#include <thread>

// clang-format off

// Constructor
TEST_CASE("Network Udp Server Tests", "[constructor]") {
  asio::io_context io_context;
  unid::network::Udp_server udp_server(io_context, 12346);

  // Run in a separate thread
  std::thread io_thread([&io_context]() {
    io_context.run();
  });

  short port = udp_server.get_port();

  REQUIRE(port == 12346);

  io_context.stop();

  if (io_thread.joinable()) {
    io_thread.join();
  }
}

// clang-format on
