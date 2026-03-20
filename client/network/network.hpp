/**
 * @file
 * @brief Network class header
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#ifndef NETWORK_H
#define NETWORK_H

#include <asio.hpp>

#include <iostream>

namespace unid::network {
class Network {
public:
  Network(){};
  ~Network(){};

  void print_network() { std::cout << "unid::network::Network\n"; }

protected:
private:
};
} // namespace unid::network

#endif // NETWORK_H
