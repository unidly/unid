/**
 * @file
 * @brief Entry point for Unid Server
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "network/udp_client.hpp"

#include "asio.hpp"

#include <iostream>

int main(int argc, char *argv[]) {
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
