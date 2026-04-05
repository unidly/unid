/**
 * @file
 * @brief Logging subclass that wraps a quill::logger instance
 * @copyright (C)opyright 2016, Unidly, LLC. All rights reserved.
 */

#ifndef QUILL_LOGGER_HPP
#define QUILL_LOGGER_HPP

#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include <memory>
#include <string>

class Quill_logger {
public:
  Quill_logger(const std::string &loggerName, const std::string &filename);
  ~Quill_logger();

  /**
   * @brief Get a pointer to the logger
   *
   * In order to use the macros for best performance, we need a pointer to the
   * logger that was created and stored in the class. This function returns
   * the logger pointer that can be used by macros, injected into other
   * classes for logging operations.
   *
   * @returns Pointer to logger that can be used with the macros
   */
  quill::Logger *get_logger() { return l_; }

  /**
   * @brief Sets the logging level
   *
   * @param level The log level
   */
  void set_log_level(quill::LogLevel level);

  /**
   * @brief Flushes the log
   *
   * This function blocks the calling thread until the backend has finished
   * processing and writing all pending log entries.
   */
  void flush_log();

  /**
   * @brief Sets immediate flush after every log call
   *
   * Some debugging operations benefit from synchronous logging. This function
   * enables synchronous logging where each log message is completely processed
   * by the backing before returning.
   *
   * @note This feature can be completely disabled at compile time by setting
   * QUILL_ENABLE_IMMEDIAT_FLUSH=0. This results in better performance as it
   * removes a conditional in the critical path.
   */
  void set_immediate_flush();

  /**
   * @brief Member logging functions
   *
   * These functions allow the logging of messages at the levels indicated
   * by the function name.
   *
   * @note These functions are implemented in this header to allow the
   * compiler to inline the code for improved performance.
   *
   * @param message The message to be logged
   */
  void trace_l3(std::string_view message) { LOG_TRACE_L3(l_, "{}", message); }
  void trace_l2(std::string_view message) { LOG_TRACE_L2(l_, "{}", message); }
  void trace_l1(std::string_view message) { LOG_TRACE_L1(l_, "{}", message); }
  void debug(std::string_view message) { LOG_DEBUG(l_, "{}", message); }
  void info(std::string_view message) { LOG_INFO(l_, "{}", message); }
  void warning(std::string_view message) { LOG_WARNING(l_, "{}", message); }
  void error(std::string_view message) { LOG_ERROR(l_, "{}", message); }
  void critical(std::string_view message) { LOG_CRITICAL(l_, "{}", message); }

private:
  quill::Logger *l_;
};

#endif // QUILL_LOGGER_HPP
