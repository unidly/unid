/**
 * @file
 * @brief A UDP client class that uses ASIO for asynchronous I/O.
 *
 * @copyright Copyright 2026 by Unidly LLC. All rights reserved.
 */

#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include "asio.hpp"
#include <array>
#include <cstdlib>
#include <iostream> // TODO remove

using asio::ip::udp;

/**
 * @class Udp_client
 *
 * Provides socket i/o using udp. Uses asio to manage the interface with the
 * socket.
 */
class Udp_client {
public:
  Udp_client(asio::io_context &io_context, const std::string &host,
             const std::string &service)
      : io_context_(io_context), socket_(io_context) {
    // 1. Resolve the remote endpoint synchronously (for simplicity in client
    // setup)
    udp::resolver resolver(io_context_);
    udp::endpoint receiver_endpoint =
        *resolver.resolve(udp::v4(), host, service).begin();

    // 2. Open the socket
    socket_.open(udp::v4());

    // 3. Start the first asynchronous send operation
    std::string message = "Hello from Asio Async UDP Client!";
    // The message data must live until the handler is called.
    // For simplicity here, we use a class member to store the message data.
    // A robust solution might use std::shared_ptr and pass it by value to the
    // handler.
    send_buf_ = std::array<char, 128>(); // Reset the buffer
    std::copy(message.begin(), message.end(), send_buf_.begin());

    socket_.async_send_to(
        asio::buffer(send_buf_, message.length()), receiver_endpoint,
        std::bind(&Udp_client::handle_send_to, this, std::placeholders::_1,
                  std::placeholders::_2, receiver_endpoint));
  }

private:
  void handle_send_to(const std::error_code &error,
                      std::size_t bytes_transferred,
                      udp::endpoint receiver_endpoint) {
    if (!error && bytes_transferred > 0) {
      std::cout << "Sent " << bytes_transferred << " bytes to "
                << receiver_endpoint.address().to_string() << ":"
                << receiver_endpoint.port() << std::endl;
      // Now that data is sent, initiate an asynchronous receive operation to
      // get the echo
      start_receive();
    } else {
      std::cerr << "Send failed: " << error.message() << std::endl;
    }
  }

  void start_receive() {
    socket_.async_receive_from(asio::buffer(recv_buffer_), remote_endpoint_,
                               std::bind(&Udp_client::handle_receive_from, this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));
  }

  void handle_receive_from(const std::error_code &error,
                           std::size_t bytes_transferred) {
    if (!error && bytes_transferred > 0) {
      std::cout << "Received " << bytes_transferred << " bytes from "
                << remote_endpoint_.address().to_string() << ":"
                << remote_endpoint_.port() << std::endl;
      std::cout << "Message: ";
      std::cout.write(recv_buffer_.data(), bytes_transferred);
      std::cout << std::endl;

      // Optional: You can initiate another send/receive cycle here or stop
      // the io_context io_context_.stop(); // Stop the application after
      // receiving the echo
    } else {
      std::cerr << "Receive failed: " << error.message() << std::endl;
    }
  }

  asio::io_context &io_context_;
  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::array<char, 128> send_buf_;
  std::array<char, 128> recv_buffer_;
};

#endif // UDP_CLIENT_HPP
