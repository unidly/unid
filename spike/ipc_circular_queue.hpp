/**
 * @file
 * @brief A circular queue that operates in shared memory IPC.
 *
 * This circular queue is designed to work within a single-provider single-
 * consumer inter process communication (IPC). The queue is located in shared
 * memory that is accessible by the two processes. The design does not use
 * traditional lock semaphores, and is non-blocking. It makes use of C++
 * atomicity.
 */

#ifndef IPC_CIRCULAR_QUEUE_HPP
#define IPC_CIRCULAR_QUEUE_HPP

#include <atomic>
#include <cstdint>
#include <iostream>   // For print state
#include <string>     // For print state

/**
 * @brief Circular queue class for memory based IPC.
 *
 * This is a purpose-built class for a unid-client that wishes to pass
 * a unid to an application program using shared memory. The producer thread
 * enqueues unids to fill the queue. and the consumer thread dequeues unids
 * as needed. The queue is sized to provide appropriate buffering of unids
 * allowing extremely fast performance and efficient delivery.
 *
 * @note This class has two requirements for its use: 1) Only the producer
 * thread can call the enqueue() function, 2) Only the consumer thread can
 * call the dequeue() function, and 3) There can only be one producing thread
 * and one consumer thread when using this class.
 *
 * @note was_empty() and was_full() are not atomic calls, this means that there
 * is no guarantee when used in a conditional statement that the return value
 * will be valid.
 */
class Circular_queue {
 public:
  /**
   * @brief Constructors and destructor for the class
   *
   * @parm cap The capacity of the shared memory queue of unids
   */
  Circular_queue(std::uint32_t cap);
  ~Circular_queue();

  /**
   * @brief Adds a unid to the shared memory circular queue.
   *
   * The producer adds unids to the queue at the rear.
   *
   * @note Only the producer thread should call this function, otherwise
   * behavior is undefined.
   *
   * @param[in] unid 64-bit unique identifier
   * @return True for success, False for queue is full
   */
  bool enqueue(const uint64_t& unid);  /**< Used by producer thread (only) */

  /**
   * @brief Removes a unid from the shared memory circular queue.
   *
   * The consumer takes unids from the queue from the front.
   *
   * @note Only the consumer thread should call this function, otherwise
   * behavior is undefined.
   *
   * @param[out] unid 64-bit unique identifier
   * @return True for success, False for queue is full
   */
  bool dequeue(uint64_t& unid);        /**< Used by the consumer thread (only) */

  /**
   * @brief Checks for empty queue
   *
   * This is non-atomic check. Close, but no cigar.
   */
  bool was_empty() const;              /**< Not atomic! */

  /**
   * @brief Checks for a full queue
   *
   * This is non-atomic check. Close, but no cigar.
   */
  bool was_full() const;               /**< Not atomic! */

  /**
   * @brief Checks to see that front and rear are both unlocked.
   *
   * TODO - determine usefulness of this member function
   */
  bool is_lock_free() const;

  /**
   * @brief Prints the queue state
   */
  void print_queue_state(const std::string message) {
    std::cout << "---------------------------------\n";
    std::cout << message << std::endl;
    std::cout << "front: " << front << std::endl;
    std::cout << "rear: " << rear << std::endl;
    std::cout << "capacity: " << capacity << std::endl;
    std::cout << "size: " << size << std::endl;
    for (std::uint32_t  i = 0; i < capacity; ++i) {
      if (i == front and i == rear) {
        std::cout << "value[" << i << "]: " << queue[i] << " <- Front <- Rear"
          << std::endl;
      }
      else if (i == front) {
        std::cout << "value[" << i << "]: " << queue[i] << " <- Front"
          << std::endl;
      } else if (i == rear) {
        std::cout << "value[" << i << "]: " << queue[i] << " <- Rear"
          << std::endl;
      } else {
        std::cout << "value[" << i << "]: " << queue[i] << std::endl;
      }
    }
  }

 private:
  /**
   * @brief Private member function to increment the front or rear index
   *
   * Queue indexes, front and rear, are incremented during the enqueue
   * and deque functions. This function performs then increment, and takes
   * care of wrapping the circular queue.
   *
   * @param index The index value
   * @return The incremented index
   */
  std::uint32_t increment(std::uint32_t index) const;

  std::uint64_t* queue;                /**< The shared memory queue ptr */
  std::atomic<std::uint32_t> front = 0;    /**< Consumer dequeues from here */
  std::atomic<std::uint32_t> rear = 0;     /**< Produce enqueues from here */
  std::uint32_t capacity;              /**<  The maximum number of unids */
  std::uint32_t size = 0;                  /**< Current number of unids */
};

#endif // IPC_CIRCULAR_QUEUE_HPP
