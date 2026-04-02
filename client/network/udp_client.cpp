/**
 * @file
 * @brief UDP client implementation using asio asynchronouse I/O
 *
 * @copyright Copyright 2026 by Unidly, LLC. All rights reserved.
 */

#include "./udp_client.hpp"

#include <asio.hpp>

// Constructor()
Udp_client::Udp_client(asio::io_context &io_context, const std::string &host,
                       const std::string &service)
    : io_context_(io_context), socket_(io_context) {
  // Server name resolution - this is a blocking call
  udp::resolver resolver(io_context_);
  udp::endpoint receiver_endpoint =
      *resolver.resolve(udp::v4(), host, service).begin();

  // Open the socket
  socket_.open(udp::v4());
}

// Destructor()
Udp_client::~Udp_client() { socket_.close(); }

// receive_from()
void Udp_client::receive_from(char *buffer, size_t length) {
  Udp_client::rx_buffer_ = buffer;
  Udp_client::rx_length_ = length;
  Udp_client::async_receive(); // Start the receiver
}

// send_to()
void Udp_client::send_to(const char *buffer, size_t length,
                         const struct sockaddr_in &client_address) {
  Udp_client::tx_buffer_ = buffer;
  Udp_client::tx_length_ = length;
  Udp_client::async_receive(); // Start the receiver
}

// async_receive()
void Udp_client::async_receive() {
  socket_.async_receive_from(
      asio::buffer(Udp_client::rx_buffer_, Udp_client::rx_length_),
      Udp_client::server_endpoint_, Udp_client::async_receive_callback_);
}

// async_send()
void Udp_client::async_send() {
  socket_.async_send_to(asio::buffer(tx_buffer_, Udp_client::tx_length_),
                        Udp_client::server_endpoint_,
                        Udp_client::async_send_callback_);
}
