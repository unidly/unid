#include <algorithm>
#include <array>
#include <iostream>

// Generic callback
template <typename F>
void process_data(std::array<char, 4000> &data, unsigned int length,
                  F callback) {
  callback(data, length);
}

// A free function that can be used as a callback
void display_buffer(std::array<char, 4000> &data, unsigned int &length) {
  for (int i = 0; i < length; ++i) {
    std::cout << data[i];
  }
  std::cout << std::endl;
  return;
}

int main() {
  std::array<char, 4000> buffer = {'H', 'e', 'l', 'l', 'o', ' ',
                                   'W', 'o', 'r', 'l', 'd', '!'};
  unsigned int buffer_len = 12;

  process_data(buffer, buffer_len, &display_buffer);

  return 0;
}
