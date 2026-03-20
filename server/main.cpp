/**
 * @file
 * @brief Entry point for Unid Server
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "asio.hpp"

#include "network/udp_server.hpp"

#include "quill/LogFunctions.h"
#include "quill/SimpleSetup.h"

#include <iostream>

#include "gsl/gsl"

int main(int argc, char *argv[]) {
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
