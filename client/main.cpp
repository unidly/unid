/**
 * @file
 * @brief Entry point for Unid Client
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "common/config.hpp"
#include "common/mempool.hpp"
#include "common/quill.hpp"
#include "network/udp_client.hpp"

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>

#include <cstddef>
#include <string>
#include <thread>

#include <iostream>

extern quill::Logger* global_logger_a;

int main(int argc, char* argv[]) {
  // Start file logging and set log level to debug for development
  setup_quill("unid_client.log");
  LOG_INFO(global_logger_a, "Starting Unid Client v0.0.1");
  global_logger_a->set_log_level(quill::LogLevel::Debug);

  // Load startup configuration
  Config config(global_logger_a);
  auto host = config.get_as<std::string>("network.client.host");
  auto service = config.get_as<std::string>("network.client.service");
  auto pool_size = config.get_as<std::size_t>("network.mempool.pool_size");
  auto chunk_size = config.get_as<std::size_t>("network.mempool.chunk_size");

  // Initialize the memory pool
  Mempool mempool(pool_size.value(), chunk_size.value(), global_logger_a);

  // Setup to run Udp_client async functions on a separate thread
  asio::io_context io_context;
  auto work_guard = asio::make_work_guard(io_context);
  std::thread io_thread([&io_context]() { io_context.run(); });

  Udp_client c(io_context, host.value(), service.value(), mempool,
               global_logger_a);

  std::cout << "Remote_port:  " << c.get_remote_port() << std::endl;
  std::cout << "Remote_address: " << c.get_remote_address() << std::endl;

  // Send data to server
  char* chunk = mempool.alloc();
  chunk[0] = 1;
  chunk[1] = 2;
  chunk[2] = 3;
  chunk[3] = 4;
  chunk[4] = 5;
  std::size_t length = 5;
  c.async_send(chunk, length);

  // Wait here and pretend you are doing something
  std::cout << "Press Enter to exit...";
  std::cin.get();

  // Order here is important
  work_guard.reset();
  io_context.stop();
  if (io_thread.joinable()) {
    io_thread.join();
  }

  LOG_INFO(global_logger_a, "Stopping Unid Client v0.0.1");
  return 0;
}
