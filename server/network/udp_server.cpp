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
    : socket_(io_context, udp::endpoint(udp::v4(), port)) {}

// Destructor()
Udp_server::~Udp_server() {
  // TODO this destructor must assure that all ASIO threads have been
  // completed and freed prior to closing the ASIO socket
  socket_.close();
}

// receive_from()
ssize_t Udp_server::receive_from(char *buffer, std::uint16_t length,
                                 struct sockaddr_in &client_address) {
  Udp_server::rx_buffer_ = buffer;
  Udp_server::rx_max_length_ = length;
  Udp_server::async_receive(); // Start the receiver
  return length;
}

// send_to()
ssize_t Udp_server::send_to(const char *buffer, std::uint16_t length,
                            const struct sockaddr_in &client_address) {
  Udp_server::tx_buffer_ = buffer;
  Udp_server::tx_max_length_ = length;
  Udp_server::async_receive(); // Start the receiver
  return length;
}

// async_receive()
void Udp_server::async_receive() {
  socket_.async_receive_from(
      asio::buffer(Udp_server::rx_buffer_, Udp_server::rx_max_length_),
      Udp_server::sender_endpoint_, Udp_server::async_receive_callback_);
}

// async_send()
void Udp_server::async_send(std::size_t length) {
  socket_.async_send_to(asio::buffer(tx_buffer_, Udp_server::tx_max_length_),
                        Udp_server::sender_endpoint_,
                        Udp_server::async_send_callback_);
}
