/**
 * @file
 * @brief UDP client implementation using asio asynchronouse I/O
 *
 * @copyright Copyright 2026 by Unidly, LLC. All rights reserved.
 */

#include "./udp_client.hpp"

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include "common/mempool.hpp"

#include <asio.hpp>
#include <cstddef>
#include <iostream>

// Constructor()
Udp_client::Udp_client(asio::io_context& io_context, const std::string& host,
                       const std::string& service, Mempool& mempool,
                       quill::Logger* logger)
    : io_context_(io_context), socket_(io_context),
      mempool_(mempool), logger_{logger} {
  LOG_INFO(logger_, "Udp_client()");

  // Get mempool chunk size
  auto chunk_size_ = (mempool_.stats()).chunk_sz;

  // Define the server endpoint
  udp::resolver resolver(io_context_);
  remote_endpoint_ = *resolver.resolve(udp::v4(), host, service).begin();

  // Open the socket and start listening
  socket_.open(udp::v4());
  async_receive();
}

// Destructor()
Udp_client::~Udp_client() {
  LOG_INFO(logger_, "~Udp_client()");
  socket_.close();
}

// async_receive()
void Udp_client::async_receive() {
  LOG_DEBUG(logger_, "async_receive() enter");
  auto chunk = mempool_.alloc();
  socket_.async_receive_from(
      asio::buffer(chunk, chunk_size_), remote_endpoint_,
      [this, chunk](std::error_code ec, std::size_t bytes_received) {
        if (!ec) {
          // Copy data into the application buffer
          std::cout << "async_receive() complete" << std::endl;
          std::cout << "Receive buffer" << std::endl;
          for (int i = 0; i < bytes_received; ++i) {
            std::cout << std::hex << std::setw(2) << std::setfill('0')
                      << static_cast<int>(chunk[i]) << std::endl;
          }
          mempool_.free(chunk); // Free the chunk after used
          async_receive();
        } else {
          // Handle error code
          std::cerr << "Receive failed: " << ec.message() << std::endl;
        }
      });
  LOG_DEBUG(logger_, "asynch_receive() exit");
}

// async_send()
void Udp_client::async_send(char* chunk, size_t length) {
  LOG_DEBUG(logger_, "async_send() enter");
  socket_.async_send_to(
      asio::buffer(chunk, length), remote_endpoint_,
      [this, chunk](std::error_code ec, std::size_t bytes_sent) {
        if (!ec) {
          std::cout << "\nasync_send() complete " << bytes_sent << " bytes sent"
                    << std::endl;
          mempool_.free(chunk);
        } else {
          // Handle error code
          std::cerr << "\nSend failed: " << ec.message() << std::endl;
        }
      });
  LOG_DEBUG(logger_, "async_send() exit");
}
