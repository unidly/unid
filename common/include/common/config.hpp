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

#include <filesystem>

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
 * The configuration values are stored as key:value pairs. Values, are stored
 * as strings, that are converted to the proper datatype when accessed using
 * helper functions.
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
   */
  Config(quill::Logger* logger);

  /**
   * @brief Config class destructor
   */
  ~Config();

  /**
   * @brief Retrieves a configuration value of a given type with default
   *
   * This function accesses the toml++ configuration table, and returns the
   * value of a given key. If they key is not found, then the default value
   * is returned.
   *
   * @tparam T The type of the value
   * @param key The key of the key:value pair
   * #param default The default value
   * @return The value  of the key:value pair, or the default
   */
  template <typename T>
  T get_as(std::string_view key, T&& default_value) const {
    return config_[key].value_or(std::forward<T>(default_value));
  }

  /**
   * @brief Retrieves a nested configuration with default
   *
   * This function accesses the toml++ configuration table, and returns the
   * value of a given key within a given section. If they key is not found,
   * then the default value is returned.
   *
   * Toml designates sections within the file as a "table". So a toml::table
   * has within it additional embedded tables providing a hierarchy for
   * accessing data. These tables are implemented as hash tables internally.
   *
   * Here is a simple example:
   *
   * ```
   * [database]
   * server = "192.168.1.1"
   * port = 5432
   * name = "production_db"
   * user = "admin"
   * password = "${DB_PASSWORD}"  # Use env variables for secrets
   * enabled = true
   * connection_max = 5000
   * ```
   * auto port = get_as<unsigned short>("database", "port", 2334);
   * returns 5432
   *
   * @tparam T The data type of the value and the default_value
   * @param table The key of the key:value pair
   * @param key The key of the key:value pair
   * #param default The default value
   * @return The value  of the key:value pair, or the default
   *
   */
  template <typename T>
  T get_as(std::string_view table, std::string_view key,
           T&& default_value) const {
    return config_[table][key].value_or(std::forward<T>(default_value));
  }

private:
  /**
   * @brief Returns the path where unid can access the unid.toml file
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
  std::string get_unid_filepath();

  quill::Logger* logger_; /**< Logger object */
  toml::table config_;    /**< Configuration data in toml++ format*/
};

#endif // CONFIG_HPP
