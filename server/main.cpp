/**
 * @file
 * @brief Entry point for Unid Server
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "network/udp_server.hpp"

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
  LOG_INFO(global_logger_a, "Starting Unid Server");

  // Load startup configuration

  try {
    if (argc != 2) {
      std::cerr << "Usage: async_udp_echo_server <port>\n";
      return 1;
    }

    asio::io_context io_context;

    Udp_server s(io_context, std::atoi(argv[1]));

    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
