/**
 * @file
 * @brief Unit tests for the Utility class
 */
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>

#include "common/utility.hpp"

#include "common/quill.hpp"
#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <asio.hpp>
#include <iostream>
#include <thread>

extern quill::Logger* global_logger_a;

// clang-format off

// Constructor
TEST_CASE("Constructor", "[utility]") {
  setup_quill("unid_test.log");
  REQUIRE_NOTHROW(Utility(global_logger_a));
}

TEST_CASE("get_as()", "[utility]") {
  setup_quill("unid_test.log");
  Utility u(global_logger_a);

  std::string value = u.get_environment_variable("HOME", std::string("Not home"));
  REQUIRE(value == "/home/msellers");

  value = u.get_environment_variable("NOT_DEFINED", std::string("Not home"));
  REQUIRE(value == "Not home");

  int intval = u.get_environment_variable("HISTSIZE", 1234);
  REQUIRE(intval == 1000);

  intval = u.get_environment_variable("NOT_DEFINED", 1234);
  REQUIRE(intval == 1234);

  /*
  int hist_size = u.get_environment_variable<int>("HISTSIZE", 1234);
  REQUIRE(hist_size == 1000);

  hist_size = u.get_environment_variable<int>("NOT_DEFINED", 1234);
  REQUIRE(hist_size == 1234);
  */
}
