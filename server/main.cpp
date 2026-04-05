/**
 * @file
 * @brief Entry point for Unid Server
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "logger/quill_logger.hpp"
#include "network/libpool.h"
#include "network/udp_server.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>

#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
  // Instantiate logger
  Quill_logger *logger_class = new Quill_logger("unid", "unid.log");
  quill::Logger *logger = logger_class->get_logger();
  LOG_INFO(logger, "Starting Unid Server");

  // Load startup configuration
  short port = 443;

  asio::io_context io_context;

  // Register send and receive callback functions
  auto async_send_callback = [&logger](const asio::error_code &ec,
                                       std::size_t bytes_transferred) {
    if (!ec) {
      LOG_DEBUG(logger, "async_send_callback registered");
    } else {
      LOG_ERROR(logger, "async_send_callback error: {}", ec.message());
    }
  };

  auto async_receive_callback = [&logger](const asio::error_code &ec,
                                          std::size_t bytes_transferred) {
    if (!ec) {
      LOG_DEBUG(logger, "async_receive_callback registered");
    } else {
      LOG_ERROR(logger, "async_receive_callback error: {}", ec.message());
    }
  };

  // Start the server
  Udp_server s(io_context, port, logger);

  // Register the callbacks
  s.set_async_receive_callback(async_receive_callback);
  s.set_async_send_callback(async_send_callback);

  // Start the udp asynch server
  io_context.run();

  // Wait here and pretend you are doing something
  std::cout << "Press Enter to exit...";
  std::cin.get();

  return 0;
}
