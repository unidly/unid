/**
 * @file
 * @brief C++ wrapper for the ansi C libpool library
 */

extern "C" {
#include "libpool.h"
}

/**
 * @class Libpool_wrapper
 * @brief Allocates and manages blocks of memory for use as buffers
 *
 * This class wraps the [libpool C library](https://github.com/8dcc/libpool).
 */
class Libpool_wrapper {
public:
  Libpool_wrapper(size_t pool_size, size_t chunk_size, Pool *pool)
      : pool_size_(pool_size), chunk_size_(chunk_size) {
    pool_ = pool_new(pool_size, chunk_size);
  }

  ~Libpool_wrapper() {}

  struct Free_deleter {
    void operator()(void *ptr) const noexcept { std::free(ptr); }
  };

  /**
   * @brief Create a memory pool
   *
   * Creates a standard fixed sized memory pool and returns a smart pointer
   * that can be used to obtain chunks of memory for use as buffers. The pool
   * is automatically the unique_ptr's destructor will automatically handle
   * deallocation when it goes out of scope.
   *
   * @returns A pool id used for all pool operations
   */
  std::unique_ptr<Pool> create_pool() {
    Pool *pool = pool_new(pool_size_, chunk_size_);
    std::unique_ptr<Pool, Free_deleter> unique_pool_ptr(pool);
    return unique_pool_ptr;
    ;
  }

  /**
   * @brief Allocates a chunk of memory to the caller
   *
   * @param pool Pointer to pool structure
   */
  void allocate_chunk() {}

  void free_chunk() {}

  void expand() {}

private:
  std::size_t pool_size_;  // How many chunks in the pool
  std::size_t chunk_size_; // How large are the chunks (in bytes)
}
