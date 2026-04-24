/**
 * @file
 * @brief Unit tests for the UDP client class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "common/quill.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include "../client/network/udp_client.hpp"
#include <asio.hpp>
#include <iostream>
#include <thread>

extern quill::Logger *global_logger_a;

// clang-format off

// Constructor
TEST_CASE("Network Udp Client Tests", "[constructor]") {
  // Set the logger
  setup_quill("unid_test.log");

  asio::io_context io_context;
  std::string host("127.0.0.1");
  std::string service("12346");
  Udp_client udp_client(io_context, host, service, global_logger_a);

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
