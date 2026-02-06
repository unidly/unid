/**
 * @file
 * @brief Implements a circular queue of 64-bit unsigned integers
 */

#include <iostream>
#include <stdint.h>

/**
 * @brief Class that implements a simple circular queue
 *
 * A circular queue is a linear data structure that operates on the First-In,
 * First-Out (FIFO) principle, where the last position is logically connected
 * back to the first, forming a circle.
 * 
 * The data stored in this queue are of type std::uint64_t
 */
class Circular_queue {
  public:
    // Constructor
    Circular_queue(std::uint32_t k) {
      capacity = k;
      arr = new std::uint64_t[capacity];
      front = 0;
      rear = -1;
      size = 0;
    }

    // Destructor to free dynamically allocated memory
    ~Circular_queue() {
      delete[] arr;
    }

    // Function to add an element
    bool enqueue(std::uint64_t value) {
      if (is_full()) {
        std::cout << "Queue is full." << std::endl;
        return false;
      }
      rear = (rear + 1) % capacity;
      arr[rear] = value;
      size++;
      return true;
    }

    // Function to delete an element
    bool dequeue() {
      if (is_empty()) {
        std::cout << "Queue is empty." << std::endl;
        return false;
      }
      // Element is retrieved, front moves to the next position
      front = (front + 1) % capacity;
      size--;
      return true;
    }

    // Function to check if the queue is empty
    bool is_empty() {
      return size == 0;
    }

    // Function to check if the queue is full
    bool is_full() {
      return size == capacity;
    }

    // Function to report the state of the queue
    void report(const std::string& msg) {
      std::cout << msg << std::endl;
      std::cout << "    capacity: " << capacity << std::endl;
      std::cout << "    size: " << size << std::endl;
      std::cout << "    is_empty(): " << ((is_empty()) ? "True" : "False") << std::endl;
      std::cout << "    is_full(): " << ((is_full()) ? "True" : "False") << std::endl;
      std::cout << "-----------------------\n";
    }

  private:
    std::int32_t front;        /**< Data is taken from front */
    std::int32_t rear;         /**< Data is added to the rear */
    std::int32_t capacity;     /**< Total Number of data elements */
    std::int32_t size;         /**< Current number of enqueued elements */
    std::uint64_t* arr;         /**< Circular queue ptr */
};

int main() {
  auto q = Circular_queue(1024);
  q.report("1. Queue initialized");

  for (int i = 0; i < 1024; i++) {
    q.enqueue(i);
  }
  q.report("2. Queue filled");

  for (int i = 0; i < 1024; i++) {
     q.dequeue();
  }
  q.report("3. Queue emptied");

  for (int i = 0; i < 512; i++) {
    q.enqueue(i);
  }
  q.report("4. Queue half filled");

  return 0;
}
