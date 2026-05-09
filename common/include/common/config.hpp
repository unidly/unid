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
#include <cstdint>
#include <filesystem>

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
 * into their C++ datatype conversions when accessed
 * * String
 * * Integer
 * * Float
 * * Boolean
 * * Datetime
 * * Date
 * * Time
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
   * @brief Retrieves a config value
   *
   * This primary template function accesses the toml++ configuration table
   * and returns the value associated with the key.
   *
   * @note Toml++ represents all integer values with int64_t. If the desired
   * datatype is a shorter type, such as int, or of a different sign, such as
   * unsigned, the return value should be static_cast<>() to match the
   * desired data type..
   *
   * @tparam T The data type of the value (e.g. int64_t, double, boolean, etc)
   * @param table The name of the table. Must be an empty string for root.
   * @param key The key of the key:value pair
   * @return The value  of the key:value pair, or the default
   */
  template <typename T>
  inline std::optional<T> get_as(std::string_view table,
                                 std::string_view key) const {
    if (table.empty()) {
      return config_[key].value<T>();
    } else {
      return config_[table][key].value<T>();
    }
  }

private:
  /**
   * @brief Returns the path of the unid.toml file
   *
   * The location of the unid.toml configuration can be located in one of two
   * places: ~/.config/unid.toml or /etc/unid/unid.toml. If the file is
   * found in the home directory, that file us used as it has priority over the
   * system configuration file. If there is no ~/.config/Unid.toml or of $HOME
   * is not set, then /etc/unid/unid.toml is used.
   *
   * @note The use of the home directory based configuration file should only
   * be used during development. Outside of the development environment, the
   * /etc/unid/unid.toml file should be used and the unid.toml file should be
   * removed from ~/.config
   *
   * @return Filepath to unid.toml
   * @throws runtime exception if configuration file not found.
   */
  std::string get_unid_filepath() const;

  quill::Logger* logger_; /**< Logger object */
  toml::table config_;    /**< Configuration data in toml++ format*/
};

#endif // CONFIG_HPP
