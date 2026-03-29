/**
 * @file
 * @brief Entry point for Unid Client
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "network/udp_client.hpp"

#include "asio.hpp"

#include "logger/quill_static.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <iostream>

extern quill::Logger *global_logger_a;

int main(int argc, char *argv[]) {
  // Start file logging
  setup_quill("unid.log");
  global_logger_a->set_log_level(quill::LogLevel::TraceL3);
  LOG_INFO(global_logger_a, "Starting Unid Client");

  // Load startup configuration

  try {
    if (argc != 3) {
      std::cerr << "Usage: udp_client <host> <service (port number)>"
                << std::endl;
      return 1;
    }

    asio::io_context io_context;
    // Create the client object, which initiates the async operations
    Udp_client client(io_context, argv[1], argv[2]);

    // Run the io_context to start the asynchronous operations
    std::cout << "Starting io_context.run()" << std::endl;
    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  return 0;
}
