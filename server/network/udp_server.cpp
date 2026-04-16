/**
 * @file
 * @brief UDP server implementation using asio asynchronouse I/O
 *
 * @copyright Copyright 2026 by Unidly, LLC. All rights reserved.
 */

#include "./udp_server.hpp"

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>

// Constructor()
Udp_server::Udp_server(asio::io_context &io_context, short port,
                       quill::Logger *logger)
    : socket_(io_context, udp::endpoint(udp::v4(), port)), logger_{logger} {
  LOG_DEBUG(Udp_server::logger_, "Udp_server()");
}

// Destructor()
Udp_server::~Udp_server() {
  LOG_DEBUG(Udp_server::logger_, "~Udp_server()");
  socket_.close();
}

// receive_from()
void Udp_server::receive_from(char *buffer, size_t length) {
  Udp_server::rx_buffer_ = buffer;
  Udp_server::rx_length_ = length;
  Udp_server::async_receive(); // Start the receiver
}

// send_to()
void Udp_server::send_to(const char *buffer, size_t length,
                         const struct sockaddr_in &client_address) {
  Udp_server::tx_buffer_ = buffer;
  Udp_server::tx_length_ = length;
  Udp_server::async_receive(); // Start the receiver
}

// async_receive()
void Udp_server::async_receive() {
  socket_.async_receive_from(
      asio::buffer(Udp_server::rx_buffer_, Udp_server::rx_length_),
      Udp_server::server_endpoint_, Udp_server::async_receive_callback_);
}

// async_send()
void Udp_server::async_send() {
  socket_.async_send_to(asio::buffer(tx_buffer_, Udp_server::tx_length_),
                        Udp_server::server_endpoint_,
                        Udp_server::async_send_callback_);
}
