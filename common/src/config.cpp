/**
 * @file
 * @brief Config class implementation.
 * @copyright Copyright 2026, Unidly LLC. All rights reserved.
 */

#include "common/config.hpp"

#define TOML_HEADER_ONLY 0
#include <toml++/toml.hpp>

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <stdexcept>

Config::Config(std::string_view file_path, quill::Logger* logger)
    : logger_{logger} {
  try {
    config_ = toml::parse_file(file_path);
    LOG_INFO(logger, "Config object created");
  } catch (const toml::parse_error& err) {
    LOG_CRITICAL(logger, "Config object creation failed");
    throw std::runtime_error(err);
  }
}

Config::~Config() { LOG_INFO(logger_, "Config object destroyed"); }
