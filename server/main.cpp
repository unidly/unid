/**
 * @file
 * @brief Entry point for Unid Server
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "common/config.hpp"
#include "common/mempool.hpp"
#include "common/quill.hpp"
#include "network/udp_server.hpp"

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>

#include <cstddef>
#include <thread>

#include <iostream>

extern quill::Logger* global_logger_a;

int main(int argc, char* argv[]) {
  // Setup logger
  setup_quill("unid_server.log");
  LOG_INFO(global_logger_a, "Starting Unid Server v0.0.1");
  global_logger_a->set_log_level(quill::LogLevel::Debug);

  // Load startup configuration
  Config config(global_logger_a);
  auto port = config.get_as<short>("network.server", "port");
  auto pool_size = config.get_as<std::size_t>("network.mempool", "pool_size");
  auto chunk_size = config.get_as<std::size_t>("network.mempool", "chunk_size");

  // Initialize the memory pool
  LOG_INFO(global_logger_a, "Creating Mempool pool_size: {} chunk_size: {}",
           pool_size.value(), chunk_size.value());
  Mempool mempool(pool_size.value(), chunk_size.value(), global_logger_a);

  // Setup to run Udp_server asynk functions on a separate thread
  asio::io_context io_context;
  auto work_guard = asio::make_work_guard(io_context);
  std::thread io_thread([&io_context]() { io_context.run(); });

  // Start the server
  Udp_server s(io_context, port.value(), mempool, global_logger_a);

  // Wait here and pretend you are doing something
  std::cout << "Press Enter to exit...";
  std::cin.get();

  if (io_thread.joinable()) {
    io_thread.join();
  }

  work_guard.reset();
  LOG_INFO(global_logger_a, "Stopping Unid Server v0.0.1");
  return 0;
}
