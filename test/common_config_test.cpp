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
#include <optional>
#include <string>
#include <string_view>
#include <thread>

extern quill::Logger* global_logger_a;

using namespace std::string_literals;
using namespace std::string_view_literals;

// clang-format off

TEST_CASE("Constructor load/parse", "[config]") {
  setup_quill("unid_test.log");
  REQUIRE_NOTHROW(Config(global_logger_a));
}

TEST_CASE("get_as()", "[config]") {
  setup_quill("unid_test.log");
  global_logger_a->set_log_level(quill::LogLevel::Debug);
  Config c(global_logger_a);

  std::optional<std::string> title = c.get_as<std::string>("title"s);
  REQUIRE(title == "Unid Configuration");

  std::optional<std::int64_t> intval = c.get_as<std::int64_t>("intval"s);
  REQUIRE(intval == 12);
  intval = c.get_as<std::int64_t>("not_found"s);
  REQUIRE(intval == std::nullopt);

  std::optional<double> doubleval = c.get_as<double>("doubleval"s);
  REQUIRE(doubleval == 12.34);
  doubleval = c.get_as<double>("not_found"s);
  REQUIRE(doubleval == std::nullopt);

  std::optional<bool> booleanval = c.get_as<bool>("booleanval"s);
  REQUIRE(booleanval == true);
  booleanval = c.get_as<bool>("not_found"s);
  REQUIRE(booleanval == std::nullopt);

  std::optional<std::chrono::year_month_day> ymd = c.get_as_date("ld1");
  REQUIRE(ymd.has_value());
  int y = static_cast<int>(ymd->year());
  unsigned m = static_cast<unsigned>(ymd->month());
  unsigned d = static_cast<unsigned>(ymd->day());
  REQUIRE(y == 1979);
  REQUIRE(m == 5);
  REQUIRE(d == 27);

  std::optional<std::chrono::hh_mm_ss<std::chrono::seconds>> hms = c.get_as_time("lt1");
  REQUIRE(hms.has_value());
  long long hrs = hms->hours().count();
  long long mins = hms->minutes().count();
  long long secs = hms->seconds().count();
  REQUIRE(hrs == 7);
  REQUIRE(mins == 32);
  REQUIRE(secs == 0);

  global_logger_a->set_log_level(quill::LogLevel::Info);
}

// clang-format on
