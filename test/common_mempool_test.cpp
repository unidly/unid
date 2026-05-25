/**
 * @file
 * @brief Unit tests for the Mempool class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "common/mempool.hpp"

#include "common/quill.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>
#include <iostream>
#include <thread>

extern quill::Logger* global_logger_a;

// clang-format off

// Constructor
TEST_CASE("Mempool constructor/stats", "[mempool]") {
  // Set the logger
  setup_quill("unid_test.log");
  std::size_t chunk_sz(1024);
  std::size_t pool_sz(16);
  Mempool mempool{pool_sz, chunk_sz, global_logger_a};

  Mempool_stats stats = mempool.stats();
  REQUIRE(stats.chunk_sz == 1024);
  REQUIRE(stats.pool_sz == 16);
  REQUIRE(stats.extra_sz == 0);
}

TEST_CASE("alloc/free", "[mempool]") {
  // Set the logger
  setup_quill("unid_test.log");
  std::size_t chunk_sz(1024);
  std::size_t pool_sz(16);
  Mempool mempool{pool_sz, chunk_sz, global_logger_a};

  char* data = mempool.alloc();
  Mempool_stats stats = mempool.stats();
  REQUIRE(stats.available == 15);

  mempool.free(data);
  stats = mempool.stats();
  REQUIRE(stats.available == 16);
}

TEST_CASE("expand", "[mempool]") {
  // Set the logger
  setup_quill("unid_test.log");
  std::size_t chunk_sz(1024);
  std::size_t pool_sz(16);
  Mempool mempool{pool_sz, chunk_sz, global_logger_a};

  Mempool_stats stats = mempool.stats();
  REQUIRE(stats.pool_sz == 16);

  bool status = mempool.expand(5);
  REQUIRE(status == true);

  stats = mempool.stats();
  REQUIRE(stats.pool_sz == 16);
  REQUIRE(stats.extra_sz == 5);
  REQUIRE(stats.available == 21);

  status = mempool.expand(4);
  stats = mempool.stats();
  REQUIRE(stats.pool_sz == 16);
  REQUIRE(stats.extra_sz == 9);
  REQUIRE(stats.available == 25);
}

TEST_CASE("alloc when no chunks available", "[mempool]") {
  // Set the logger
  setup_quill("unid_test.log");
  std::size_t chunk_sz(1024);
  std::size_t pool_sz(5);
  Mempool mempool{pool_sz, chunk_sz, global_logger_a};

  // All of these allocations result in a valid pointer
  char* data1 = mempool.alloc();
  char* data2 = mempool.alloc();
  char* data3 = mempool.alloc();
  char* data4 = mempool.alloc();
  char* data5 = mempool.alloc();
  REQUIRE(data5 != NULL);

  // This allocation fails
  REQUIRE_THROWS(mempool.alloc());
}

// clang-format on
