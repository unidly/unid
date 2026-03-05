/**
 * @file
 * @brief Entry point for Unid Server
 *
 * @copyright Copyright 2026, Unidly LLC
 */

#include "main.hpp"

#include "network/network.hpp"

#include "quill/LogFunctions.h"
#include "quill/SimpleSetup.h"

#include <iostream>

#include "gsl/gsl"

int main() {
  std::cout << "main.cpp\n";

  unid::network::Network network;
  network.print_network();

  return 0;
}
