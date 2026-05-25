/**
 * @file
 * @brief Config class implementation.
 * @copyright Copyright 2026, Unidly LLC. All rights reserved.
 */

#include "common/config.hpp"
#include "common/string_constants.hpp"

#define TOML_HEADER_ONLY 0
#include <toml++/toml.hpp>

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

Config::Config(quill::Logger* logger) : logger_{logger} {
  try {
    config_ = toml::parse_file(Config::get_unid_filepath());
    LOG_INFO(logger_, "Config()");
  } catch (const toml::parse_error& err) {
    LOG_CRITICAL(logger_, "Config() failed with parse error.");
    throw std::runtime_error(err);
  }
}

Config::~Config() { LOG_INFO(logger_, "~Config()"); }

/**
 * @brief Returns the filepath of unid.toml configuration file
 *
 * The location of the unid.toml configuration can be located in one of two
 * places: ~/.config/unid.toml or /etc/unid/unid.toml. If the file is
 * found in the home directory, that file us used as it has priority over the
 * system configuration file. If there is no ~/.config/unid.toml or of $HOME is
 * not set, then /etc/unid/unid.toml is used.
 *
 * @note Cmake defines NDEBUG for production builds. This function only
 * searches the $HOME for the configuration file if the CMAKE_BUILD_TYPE is
 * set to Debug.
 *
 * @throws runtime error if configuration file not found
 */
std::string Config::get_unid_filepath() const {
  std::string filepath;

#ifndef NDEBUG
  // Try home path
  if (!home.empty()) {
    filepath = config_home_filepath;
    fs::path p{filepath};
    if (fs::exists(p)) {
      LOG_INFO(logger_, "Using config file: {}", filepath);
      return filepath;
    }
  }
#endif

  // Try system path
  filepath = config_system_filepath;
  fs::path p{filepath};
  if (!fs::exists(p)) {
    throw std::runtime_error("error: unid.toml not found");
  }

  // Not found!
  LOG_INFO(logger_, "Using config file: {}", filepath);
  return filepath;
}
