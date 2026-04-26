/**
 * @file
 * @brief Unit tests for the Config class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "common/config.hpp"

#include "common/quill.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>
#include <iostream>
#include <thread>

extern quill::Logger* global_logger_a;

// clang-format off

// Constructor
TEST_CASE("Config constructor", "[config]") {
  // Set the logger
  setup_quill("unid_test.log");
  Config config(global_logger_a);
  REQUIRE_NOTHROW(Config(global_logger_a));
}
