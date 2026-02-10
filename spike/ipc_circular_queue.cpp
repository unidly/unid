/**
 * @file
 * @brief Implementation of circular queue member functions
 */

#include "ipc_circular_queue.hpp"

#include <atomic>
#include <cstring>
#include <cstdint>
#include <iostream>   // Debug - for print state
#include <stdexcept>

/* Constructor() */
Circular_queue::Circular_queue(std::uint32_t cap) : capacity(cap) {
  if (!capacity) {
    throw std::invalid_argument("Circular_queue: Capacity must be > 0");
  }

  queue = new std::uint64_t[capacity];

  // Initialize the queue helps with testing/debug
  memset(queue, 0, capacity);
}

/* Destructor() */
Circular_queue::~Circular_queue() {
  delete[] queue;
}

/* enqueue() */
bool Circular_queue::enqueue(const std::uint64_t& unid) {
  const auto current_rear = rear.load(std::memory_order_relaxed);
  const auto next_rear = increment(current_rear);
  if (next_rear != front.load(std::memory_order_acquire)) {
    queue[current_rear] = unid;
    rear.store(next_rear, std::memory_order_release);
    size++;
    return true;
  }

  throw std::logic_error("enqueue(): Queue overflow");
  //return false;  // queue is full
}

/* dequeue() */
bool Circular_queue::dequeue(std::uint64_t& unid) {
  const auto current_front = front.load(std::memory_order_relaxed);
  if (current_front == rear.load(std::memory_order_acquire)) {
    return false;  // queue is empty
  }
  unid = queue[current_front];
  front.store(increment(current_front), std::memory_order_release);
  size--;

  return true;
}

/* was_empty() */
bool Circular_queue::was_empty() const {
  return (front.load() == rear.load());
}

/* was_full() */
bool Circular_queue::was_full() const {
  const auto next_rear = increment(rear.load());
  return (next_rear == front.load());
}

/* is_lock_free() */
bool Circular_queue::is_lock_free() const {
  return (rear.is_lock_free() && front.is_lock_free());
}

/* increment() */
std::uint32_t Circular_queue::increment(std::uint32_t index) const {
  return (index + 1) % capacity;
}

/* main() */
int main() {
  std::cout << "Starting main()\n";
  std::cout << "Functionality tests\n";
  std::cout << "0. Construct - capacity = 0\n";
  {
    try {
      Circular_queue cq(0);
    } catch (const std::invalid_argument& e) {
      std::cerr << e.what() << std::endl;
    }
  }

  {
    Circular_queue cq(5);
    cq.print_queue_state("1. Construct - capacity = 5");
  }

  {
    std::uint64_t value;
    Circular_queue cq(5);
    cq.enqueue(1);
    cq.enqueue(2);
    cq.enqueue(3);
    cq.print_queue_state("2. Enqueue 3 values");
    cq.dequeue(value);
    cq.print_queue_state("3. Dequeue 1 value");
    cq.dequeue(value);
    cq.dequeue(value);
    cq.print_queue_state("3. Dequeue 2 values - now empty");
  }

  {
    Circular_queue cq(5);
    cq.enqueue(1);
    cq.enqueue(2);
    cq.enqueue(3);
    cq.enqueue(4);
    //cq.enqueue(5);
    try {
      cq.enqueue(5);
    } catch (const std::logic_error& e) {
      std::cerr << e.what() << std::endl;
    }
    cq.print_queue_state("4. Equeue 6 values - overflow!");
  }


  return 0;
}
