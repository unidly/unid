/**
 * @file
 * @brief UDP server implementation using asio
 *
 * @copyright Copyright 2026 by Unidly, LLC. All rights reserved.
 */

#include "./udp_server.hpp"

#include "common/config.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>

// Constructor()
Udp_server::Udp_server(asio::io_context& io_context, short port,
                       Mempool& mempool, quill::Logger* logger)
    : socket_(io_context, udp::endpoint(udp::v4(), port)),
      mempool_(mempool), logger_{logger} {
  LOG_INFO(logger_, "Udp_server()");

  // Get mempool chunk size
  auto chunk_size_ = (mempool_.stats()).chunk_sz;
  remote_endpoint_ = socket_.local_endpoint();

  // Start receive
  async_receive();
}

// Destructor()
Udp_server::~Udp_server() {
  LOG_INFO(logger_, "~Udp_server()");
  socket_.close();
}

// async_receive()
void Udp_server::async_receive() {
  auto chunk = mempool_.alloc();
  socket_.async_receive_from(
      asio::buffer(chunk, chunk_size_), remote_endpoint_,
      [this, chunk](std::error_code ec, std::size_t bytes_received) {
        if (!ec) {
          std::cout << "async_receive() complete" << std::endl;
          std::cout << "Receive buffer" << std::endl;
          for (int i = 0; i < bytes_received; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(chunk[i]) << std::endl;
          }
          mempool_.free(chunk);
          async_receive();
        } else {
          // Handle error code
        }
      });
}

// async_send()
void Udp_server::async_send(char* chunk, size_t length) {
  socket_.async_send_to(
      asio::buffer(chunk, length), remote_endpoint_,
      [this, chunk](std::error_code ec, std::size_t byte_received) {
        if (!ec) {
          std::cout << "async_send() complete" << std::endl;
          mempool_.free(chunk);
        } else {
          // Handle error code
        }
      });
}
