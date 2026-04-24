/**
 * @file
 * @brief Entry point for Unid Client
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "common/quill.hpp"
#include "network/udp_client.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include "asio.hpp"

#include <iostream>

extern quill::Logger *global_logger_a;

int main(int argc, char *argv[]) {
  // Start file logging
  setup_quill("unid_server.log");

  // Load startup configuration

  asio::io_context io_context;

  // Send and receive callback
  auto async_send_callback = [](const asio::error_code &ec,
                                std::size_t bytes_transferred) {
    if (!ec) {
      LOG_DEBUG(global_logger_a, "async_send_callback registered");
    } else {
      LOG_ERROR(global_logger_a, "async_send_callback error: {}", ec.message());
    }
  };

  auto async_receive_callback = [](const asio::error_code &ec,
                                   std::size_t bytes_transferred) {
    if (!ec) {
      LOG_DEBUG(global_logger_a, "async_receive_callback registered");
    } else {
      LOG_ERROR(global_logger_a, "async_receive_callback error: {}",
                ec.message());
    }
  };

  Udp_client c(io_context, "127.0.0.1", "443", global_logger_a);

  // Register the callbacks
  c.set_async_receive_callback(async_receive_callback);
  c.set_async_send_callback(async_send_callback);

  // Start the udp asynch server
  io_context.run();

  // Wait here and pretend you are doing something
  std::cout << "Press Enter to exit...";
  std::cin.get();

  return 0;
}
