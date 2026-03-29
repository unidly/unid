#include <algorithm>
#include <iostream>
#include <vector>

// The system library/class that performs an operation and takes a generic
// callback
template <typename F> void process_data(std::vector<int> &data, F callback) {
  for (int &value : data) {
    // The compiler generates specific code for the passed-in callback type 'F'
    // This can be inlined for maximum performance (zero-cost abstraction)
    callback(value);
  }
}

// A free function that can be used as a callback
void multiply_by_two(int &x) { x *= 2; }

class Transformer {
public:
  int multiplier = 3;

  // A member function (cannot be passed directly to C-style function pointer)
  void multiply_by_three(int &x) { x *= multiplier; }
};

int main() {
  std::vector<int> numbers = {1, 2, 3, 4, 5};

  // --- Example 1: Using a free function ---
  // The compiler deduces 'F' as a function pointer type
  process_data(numbers, &multiply_by_two);

  std::cout << "After multiplying by two: ";
  for (int x : numbers) {
    std::cout << x << " ";
  } // Output: 2 4 6 8 10
  std::cout << std::endl;

  // --- Example 2: Using a generic lambda with capture ---
  // The compiler creates an optimized function object (closure)
  Transformer transformer;
  process_data(numbers,
               [&transformer](int &x) { // Captures 'transformer' by reference
                 transformer.multiply_by_three(x);
               });

  std::cout << "After multiplying by three: ";
  for (int x : numbers) {
    std::cout << x << " ";
  } // Output: 6 12 18 24 30
  std::cout << std::endl;

  // --- Example 3: Using a simple lambda ---
  process_data(numbers, [](int &x) { x += 1; });

  std::cout << "After adding one: ";
  for (int x : numbers) {
    std::cout << x << " ";
  } // Output: 7 13 19 25 31
  std::cout << std::endl;

  return 0;
}
