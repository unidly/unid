/**
 * @file
 * @brief A UDP server class that uses ASIO for asynch operation..
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include "asio.hpp"
#include <cstdlib>

#include "quill/LogMacros.h"
#include "quill/Logger.h"

using asio::ip::udp;

/**
 * @class Udp_server
 *
 * Provides socket i/o using udp. Uses asio to manage the interface with the
 * socket.
 */
class Udp_server {
public:
  Udp_server(asio::io_context &io_context, short port);
  ~Udp_server();

  void do_receive();

  void do_send(std::size_t length);

private:
  udp::socket socket_;
  udp::endpoint sender_endpoint_;
  static constexpr int max_length = 1024;
  char data_[max_length];
};

/* Example caller */
/*
int main(int argc, char *argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: async_udp_echo_server <port>\n";
      return 1;
    }

    asio::io_context io_context;

    server s(io_context, std::atoi(argv[1]));

    io_context.run();
  } catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
*/

#endif // UDP_SERVER_HPP
