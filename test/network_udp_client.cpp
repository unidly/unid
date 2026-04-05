/**
 * @file
 * @brief Unit tests for the UDP client class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../client/network/udp_client.hpp"
#include <asio.hpp>
#include <iostream>
#include <thread>

// clang-format off

// Constructor
TEST_CASE("Network Udp Client Tests", "[constructor]") {
  asio::io_context io_context;
  std::string host("127.0.0.1");
  std::string service("12346");
  Udp_client udp_client(io_context, host, service);

  // Run in a separate thread
  std::thread io_thread([&io_context]() {
    io_context.run();
  });

  io_context.stop();

  if (io_thread.joinable()) {
    io_thread.join();
  }
}

// clang-format on
