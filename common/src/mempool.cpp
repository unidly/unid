/**
 * @file
 * @brief C++ memory pool wrapper for libpool implementation
 * @copyright Copyright 2026, Unidly, LLC - All rights reserved.
 */

#include "common/mempool.hpp"

extern "C" {
#include "common/libpool.h"
}

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <cstddef>   // for std::size_t
#include <stdexcept> // for std::runtime_error

Mempool::Mempool(std::size_t pool_sz, std::size_t chunk_sz,
                 quill::Logger* logger)
    : pool_sz_(pool_sz), chunk_sz_(chunk_sz), extra_sz_(0),
      pool_(pool_new(pool_sz, chunk_sz)), logger_(logger) {
  if (!pool_) {
    LOG_CRITICAL(logger, "Failed to initialize libpool");
    throw std::runtime_error("Failed to initialize libpool");
  }
  LOG_INFO(logger, "Mempool object created");
}

Mempool::~Mempool() {
  pool_destroy(pool_);
  LOG_INFO(logger_, "Mempool object destroyed");
}

bool Mempool::expand(std::size_t extra_sz) {
  bool status = pool_expand(pool_, extra_sz);
  if (!status) {
    // Can't expand the pool - not enough memory!
    pool_destroy(pool_);
    LOG_CRITICAL(logger_, "Failed to expand mempool");
    throw std::runtime_error("Failed to expand mempool");
  }
  extra_sz_ += extra_sz;
  return status;
}

struct Mempool_stats Mempool::stats() {
  std::size_t available = pool_available(pool_);
  return {chunk_sz_, pool_sz_, extra_sz_, available};
}
