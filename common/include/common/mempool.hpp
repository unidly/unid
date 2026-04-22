/**
 * @file
 * @brief C++ memory pool wrapper for libpool (header only)
 * @copyright Copyright 2026, Unidly, LLC - All rights reserved.
 */

#ifndef MEMPOOL_HPP
#define MEMPOOL_HPP

extern "C" {
#include "libpool.h"
}

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <cstddef> // for std::size_t

/**
 * @class Mempool
 * @brief This class provides a c++ interface to libpool.
 *
 * [Libpool](https://github.com/8dcc/libpool) is a small and simple
 * memory pool manager written in C. This class wraps the memory pool
 * manager providing a c++ interface.
 *
 * @note The class adopts the naming conventions of the libpool library.
 */
class Mempool {
public:
  /**
   * @brief Class constructor
   *
   * Creates a memory pool using malloc.
   *
   * @param pool_sz Number of chunks in the pool
   * @param chunk_sz Size in bytes of each chunk
   * @param logger Pointer to the logger object
   */
  Mempool(std::size_t pool_sz, std::size_t chunk_sz, quill::Logger* logger);

  /**
   * @brief Class destructor
   *
   * Frees allocated pool memory and destroys the pool.
   */
  ~Mempool();

  /**
   * @brief Allocate a chunk of memory for the caller to use
   *
   * @return a pointer to a chunk of memory
   */
  void* mempool_alloc() { return pool_alloc(Mempool::pool_); }

  /**
   * @brief Frees a block after the caller doesn't need it anymore
   *
   * @param ptr Pointer to the chunk to free
   */
  void mempool_free(void* ptr) { pool_free(Mempool::pool_, ptr); }

  /**
   * @brief Increase the number of chunks in the pool
   *
   * @note The total number of chunks in the pool are equal to the
   * the pool_sz + extra_sz. This function can be called, multiple times
   * and the chunk_sz_ member variable accumulates each time this is
   * called.
   *
   * @param extra_sz Addition blocks added after initial pool
   * @return True OK, otherwise throws an exception
   * @throws std::runtime_error if memory allocation fails
   */
  bool mempool_expand(std::size_t extra_sz);

private:
  std::size_t pool_sz_;  /**< Number of chunks in the pool */
  std::size_t chunk_sz_; /**< Size in bytes of a chunk */
  std::size_t extra_sz_; /**< Extra chunks added to the pool (cumulative) */
  Pool* pool_;           /**< Raw pointer to the pool struct */
};

#endif // MEMPOOL_HPP
