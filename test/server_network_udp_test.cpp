/**
 * @file
 * @brief Unit tests for the UDP server class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "../server/network/udp_server.hpp"

#include "common/quill.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>
#include <iostream>
#include <thread>

extern quill::Logger* global_logger_a;

// clang-format off

// Constructor
TEST_CASE("Network Udp Server Tests", "[constructor]") {
  // Set the logger
  setup_quill("unid_test.log");

  // Set the configuration
  Config c(global_logger_a);
  Config* configure = &c;

  // Start the server
  asio::io_context io_context;
  Udp_server udp_server(io_context, 12346, global_logger_a, configure);

  // Run in a separate thread
  std::thread io_thread([&io_context]() {
    io_context.run();
  });

  short port = udp_server.get_local_port();
  REQUIRE(port == 12346);

  std::string address = udp_server.get_local_address();
  REQUIRE(address == "0.0.0.0");

  io_context.stop();

  if (io_thread.joinable()) {
    io_thread.join();
  }
}

// clang-format on
