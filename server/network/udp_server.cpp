/**
 * @file
 * @brief UDP server implementation using asio asynchronouse I/O
 *
 * @copyright Copyright 2026 by Unidly, LLC. All rights reserved.
 */

#include "./udp_server.hpp"

#include <asio.hpp>

// Constructor()
Udp_server::Udp_server(asio::io_context &io_context, short port)
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {
  do_receive();
}

// Destructor()
Udp_server::~Udp_server() { socket_.close(); }

// do_receive()
void Udp_server::do_receive() {
  socket_.async_receive_from(
      asio::buffer(data_, max_length), sender_endpoint_,
      [this](std::error_code ec, std::size_t bytes_recvd) {
        if (!ec && bytes_recvd > 0) {
          do_send(bytes_recvd);
        } else {
          do_receive();
        }
      });
}

// do_send()
void Udp_server::do_send(std::size_t length) {
  socket_.async_send_to(asio::buffer(data_, length), sender_endpoint_,
                        [this](std::error_code /*ec*/,
                               std::size_t /*bytes_sent*/) { do_receive(); });
}
