/**
 * @file
 * @brief Config class interface
 * @copyright Copyright 2026 Unidly, LLC. All rights reserved
 */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define TOML_IMPLEMENTATION
#include <toml++/toml.hpp>

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <charconv>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string_view>

#include <iostream>

/**
 * @class Config
 * @brief Manages and distributes configuration parameters to the application
 *
 * Configuration data is stored in the filesystem in Toml format. The data is
 * loaded from the filesystem, and stored in a set of unordered maps that
 * are segregated by the target area of use. For example: Database, Network,
 * Consensus, etc.
 *
 * The Config object is injected into the various consumer classes that
 * require the configuratin information.
 *
 * The following data types are supported in the toml file, and are converted
 * into their C++ datatype conversions when accessed:
 *
 * - Tables (standard)
 * - Tables (inline)
 * - Tables (array of) NOT IMPLEMENTED
 * - String
 * - Integer
 * - Float
 * - Boolean
 * - Local and zoned Datetime
 * - Local Date
 * - Local Time
 * - Array fixed data type NOT IMPLEMENTED
 * - Array mixed data types NOT IMPLEMENTED
 *
 * Within the configuration file, parameters are associated in categories,
 * called tables. The tables segregate the data into the various areas or
 * modules defined in the system, for example: network, database, consesnsus,
 * etc.
 *
 * The configuration values are stored as key:value pairs. Values, are stored
 * as strings, that are converted to the proper datatype when accessed using
 * helper functions. The keys contain only  letters, digits, underscores and
 * dashes. Space characters are not allowed.
 *
 * @todo Add support for arrays
 *
 * @note Current implementation does not provide array or object types for
 * config data storage. Only key:value pairs are supported.
 */
class Config {
public:
  /**
   * @brief Config class constructor
   *
   * Loads and parses the unid.toml file that contains the configuration
   * data.
   *
   * @param logger Pointer to logger
   * @throws Runtime error if unid.coml is not found
   */
  Config(quill::Logger* logger);

  /**
   * @brief Config class destructor
   */
  ~Config();

  /**
   * @brief Retrieves a config value for intrinsic data types
   *
   * This primary template function accesses the toml++ configuration table
   * and returns the value associated with the key.
   *
   * Supports data types:
   *
   * - int - short, unsigned short, int, unsigned int, long, unsigned long
   * - float types - float, double
   * - boolean
   * - string types - std::string, std::string_view
   *
   * @note Toml++ represents all integer values with int64_t. If the desired
   * datatype is a shorter type, such as int, or of a different sign, such as
   * unsigned, the return value should be static_cast<>() to match the
   * desired data type.
   *
   * @tparam T The data type of the value (e.g. int64_t, double, boolean, etc)
   * @param keypath The path, including the key of the key:value pair
   * @return The value of the key
   */
  template <typename T>
  std::optional<T> get_as(std::string_view keypath) const {
    return config_.at_path(keypath).value<T>();
  }

  /**
   * @brief Retrieves local date
   *
   * A local date is expressed as YYYY-MM-DD, and does not incorporate a
   * timezone offset. The value returnd is C++20 chrono year_month_day or
   * std::nullopt if the table or key value is not found.
   *
   * @note toml guarantees that the date is valid, otherwise it fails parsing
   *
   * @param table (optional) Toml table or nested table [tbl.tblnest]
   * @param key The key of the key:value pair
   * @return The value of the key:value pair, or std::nullopt
   */
  std::optional<std::chrono::year_month_day> get_as_date(std::string_view key) {
    using namespace std::chrono;

    if (!config_[key]) { // "key" not found
      return std::nullopt;
    }

    toml::date td = config_[key].as_date()->get();
    year_month_day ymd{year{(short)td.year} / month{td.month} / day{td.day}};
    return ymd;
  }

  std::optional<std::chrono::year_month_day> get_as_date(std::string_view table,
                                                         std::string_view key) {
    using namespace std::chrono;

    if (!config_[table][key]) { // "key" and/or "table" not found
      return std::nullopt;
    }

    toml::date td = config_[table][key].as_date()->get();
    year_month_day ymd{year{(short)td.year} / month{td.month} / day{td.day}};
    return ymd;
  }

  /**
   * @brief Retrieves local time
   *
   * A local time is expressesd as hh:mm:ss. The value is returned as C++
   * chrono hh_mm_ss or std::nullopt if the table or key value is not found.
   *
   * These functions offer resolution of 1 second. Sub-second representatons
   * such as hh:mm:ss.sss are not supported
   * *
   * @note toml guarantees that the time is valid, otherise it fails parsing
   *
   * @param table (optional) Toml table or nested table [tbl.tblnest]
   * @param key The key of the key:value pair
   * @return The value of the key:value pair, or std::nullopt
   */
  std::optional<std::chrono::hh_mm_ss<std::chrono::seconds>>
  get_as_time(std::string_view key) {
    using namespace std::chrono;

    if (!config_[key]) { // "key" not found
      return std::nullopt;
    }

    toml::time tt = config_[key].as_time()->get();
    hh_mm_ss hms{hours{tt.hour} + minutes{tt.minute} + seconds{tt.second}};
    return hms;
  }

  std::optional<std::chrono::hh_mm_ss<std::chrono::seconds>>
  get_as_time(std::string_view table, std::string_view key) {
    using namespace std::chrono;

    if (!config_[table][key]) { // "key" and/or "table" not found
      return std::nullopt;
    }

    toml::time tt = config_[table][key].as_time()->get();
    hh_mm_ss hms{hours{tt.hour} + minutes{tt.minute} + seconds{tt.second}};
    return hms;
  }

  /**
   * @brief Retrieves local datetime or offset datetime
   *
   * A local datetime is expressed as YY-MM-DDThh:mm:ss. The value is returned
   * as a chono timepoint.
   *
   * An offset datetime is expressed as YY-MM-DDThh:mm:ss+/-hh:mm. The value
   * is returned as on offset time from UTC.
   *
   * The resolution of the timepoint depends on the system clock. In most cases,
   * the system clock has resolution of 1ns.
   *
   * @note toml guarantees that the date_time is valid, otherise it fails
   * parsing
   *
   * @param table (optional) Toml table or nested table [tbl.tblnest]
   * @param key The key of the key:value pair
   * @return The value of the key:value pair, or std::nullopt
   */
  std::optional<std::chrono::system_clock::time_point>
  get_as_systime(std::string_view key) {
    using namespace std::chrono;

    if (!config_[key]) { // "key" not found
      return std::nullopt;
    }

    toml::date_time tdt = config_[key].as_date_time()->get();
    year_month_day date = year{tdt.date.year} / tdt.date.month / tdt.date.day;
    sys_days sd{date};
    system_clock::time_point tp =
        sd + hours{tdt.time.hour} + minutes{tdt.time.minute} +
        seconds{tdt.time.second} + nanoseconds{tdt.time.nanosecond};
    if (tdt.offset) {
      tp -= minutes{tdt.offset->minutes};
    }
    return tp;
  }

  std::optional<std::chrono::system_clock::time_point>
  get_as_systime(std::string_view table, std::string_view key) {
    using namespace std::chrono;

    if (!config_[table][key]) { // "key" and/or "table" not found
      return std::nullopt;
    }

    toml::date_time tdt = config_[key].as_date_time()->get();
    year_month_day date = year{tdt.date.year} / tdt.date.month / tdt.date.day;
    sys_days sd{date};
    system_clock::time_point tp =
        sd + hours{tdt.time.hour} + minutes{tdt.time.minute} +
        seconds{tdt.time.second} + nanoseconds{tdt.time.nanosecond};
    if (tdt.offset) {
      tp -= minutes{tdt.offset->minutes};
    }
    return tp;
  }

private:
  /**
   * @brief Returns the path of the unid.toml file
   *
   * The location of the unid.toml configuration can be located in one of
   * two places: ~/.config/unid.toml or /etc/unid/unid.toml. If the file is
   * found in the home directory, that file us used as it has priority over
   * the system configuration file. If there is no ~/.config/Unid.toml or of
   * $HOME is not set, then /etc/unid/unid.toml is used.
   *
   * @note The use of the home directory based configuration file should
   * only be used during development. Outside of the development
   * environment, the /etc/unid/unid.toml file should be used and the
   * unid.toml file should be removed from ~/.config
   *
   * @return Filepath to unid.toml
   * @throws runtime exception if configuration file not found.
   */
  std::string get_unid_filepath() const;

  quill::Logger* logger_; /**< Logger object */
  toml::table config_;    /**< Configuration data in toml++ format */
};

#endif // CONFIG_HPP
