/**
 * @file
 * @brief UDP server implementation using asio asynchronouse I/O
 *
 * @copyright Copyright 2026 by Unidly, LLC. All rights reserved.
 */

#include "./udp_server.hpp"

#include <asio.hpp>

// Constructor()
unid::network::Udp_server::Udp_server(asio::io_context &io_context, short port)
    : port_(port), socket_(io_context, udp::endpoint(udp::v4(), port_)),
      work_guard_(asio::make_work_guard(io_context)) {
  start_receive();
}

// Destructor()
unid::network::Udp_server::~Udp_server() {
  socket_.close();
  work_guard_.reset(); // Needed for a single thread?
}

// start_receive()
void unid::network::Udp_server::start_receive() {
  socket_.async_receive_from(asio::buffer(recv_buf_), remote_endpoint_,
                             std::bind(&Udp_server::handle_receive, this,
                                       std::placeholders::_1,
                                       std::placeholders::_2));
}

void unid::network::Udp_server::handle_receive(const asio::error_code &error,
                                               std::size_t bytes_transferred) {
  if (!error || error == asio::error::message_size) {
    // Process the received data (e.g., print it)
    std::cout << "Received from " << remote_endpoint_.address().to_string()
              << ":" << remote_endpoint_.port()
              << " - Msg: " << std::string(recv_buf_.data(), bytes_transferred)
              << std::endl;

    // Re-arm the receive operation to continue listening for more data
    start_receive();
  }
}
