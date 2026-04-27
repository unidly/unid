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
TEST_CASE("Constructor load/parse", "[config]") {
  setup_quill("unid_test.log");
  //Config Config(global_logger_a);
  REQUIRE_NOTHROW(Config(global_logger_a));
}

TEST_CASE("get_as()", "[config]") {
  setup_quill("unid_test.log");
  global_logger_a->set_log_level(quill::LogLevel::Debug);
  Config c(global_logger_a);

  std::string title = c.get_as<std::string>("title", "Default title");
  REQUIRE(title == "Unid Configuration");
  global_logger_a->set_log_level(quill::LogLevel::Info);
}
