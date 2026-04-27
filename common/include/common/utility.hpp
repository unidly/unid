/**
 * @file
 * @brief Utility class interface
 * @copyright Copyright 2026, Unidly LLC. All rights reserved
 */

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "quill/LogMacros.h"
#include "quill/Logger.h"

#include <cstdlib> // getenv()
#include <sstream>
#include <string>
#include <string_view>

/**
 * @class Utility
 * @brief Common utility functions
 *
 * Utility style fuctions that are needed within the Unid software are
 * implemented in this class. Class functions are organized in this file
 * alphabetically.
 *
 * Simple inline style functions are implemented in this header. For more
 * complex implementations, see config.cpp.
 */
class Utility {
public:
  Utility(quill::Logger* logger);

  /**
   * @brief Get an environment variable value or default value
   *
   * This is the primary template for get_environment_variable(). The
   * function is sub-optimal as it uses stringstream for data type
   * conversion.
   *
   * @todo Add full specialized for ints, booleans
   *
   * @return If the key is found, and the value is convertable using
   * std::stringstream, the value is returned to caller. Otherwise, the
   * default value is returned.
   * value is empty. If the key is not found, this function returns
   */
  template <typename T>
  inline T get_environment_variable(std::string_view key,
                                    T&& default_value) const {
    const char* raw_value = std::getenv(key.data());
    if (raw_value == nullptr) {
      return default_value;
    }

    T value;
    std::stringstream ss(raw_value);
    if (!(ss >> value)) {
      return default_value;
    }

    return value;
  }

private:
  quill::Logger* logger_; /**< Logger object */
};

#endif // UTILITY_HPP
