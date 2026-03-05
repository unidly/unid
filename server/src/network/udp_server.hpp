/**
 * @file
 * @brief A UDP server class that uses ASIO for asynchronous I/O
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#include <array>
#include <asio.hpp>
#include <functional>
#include <iostream>
#include <optional>
#include <string>

using asio::ip::udp;

namespace unid::network {
/**
 * @brief Udp_server class
 *
 * This class uses asio for an asynchronous, high speed connection to the
 * UDP ports.
 *
 * The calling program must set up the io_context, and pass it to the
 * constructor, along with a desired port number.
 *
 * If the port number is in use, an exception will be generated.
 *
 * After the class is instantiated, the io_context.run() function begins
 * the receive operation on the server port.
 *
 * Here is a code snippet for the caller:
 *   asio::io_context io_context;
 *   Udp_server server(io_context, 13); // Listen on UDP port 13
 *   io_context.run();
 *
 */
class Udp_server {
public:
  using work_guard_type =
      asio::executor_work_guard<asio::io_context::executor_type>;
  /**
   * @brief The Udp_server class constructor
   *
   * The constructor creates a socket that is used for i/o.
   *
   * TODO Research the use of a multi-threaded implementation of this context
   * on a pool of threads instead of a single thread for parallel processing
   * of streams coming in from Unid_clients.
   *
   * @param io_context Asio context
   * @param port UDP recieve port
   * @throws asio::error::port_in_use or std::system_error
   */
  Udp_server(asio::io_context &io_context, short port);

  /**
   * @brief The Udp_server class destructor
   *
   * Cleans up asio connection and closes all sockets
   */
  ~Udp_server();

  short get_port() { return port; };

private:
  // Initiates an asynchronous receive operation
  void start_receive();

  // Handler for received data
  void handle_receive(const asio::error_code &error,
                      std::size_t bytes_transferred);

  udp::socket socket;
  udp::endpoint remote_endpoint;
  std::optional<work_guard_type> work_guard;
  short port;
  std::array<char, 1024> recv_buf;
};

/*
// Main function to run the server
int main() {
  try {
    asio::io_context io_context;
    Udp_server server(io_context, 13); // Listen on UDP port 13

    std::cout << "UDP Server running on port 13. Press Ctrl+C to exit."
              << std::endl;
    io_context.run(); // Run the io_context to start the asynchronous operations
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
  return 0;
}
*/
} // namespace unid::network
