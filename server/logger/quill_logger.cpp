/**
 * @file
 * @brief Quill logger implementation
 *
 * @note Because of header-only nature of the Quill library, we explicitly
 * implement these functions here. This precompiles the larger and more
 * costly include files in the generation of the implemention and reduces
 * the overall compile time for the application.
 *
 * @copyright (C)opyright 2026, Unidly, LLC. All rights reserved.
 */
#include "quill_logger.hpp"

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/FileSink.h"
#include "quill/sinks/RotatingFileSink.h"

#include <string>
#include <string_view>

Quill_logger::Quill_logger(const std::string &loggerName,
                           const std::string &filename) {
  // Start the backend thread
  quill::Backend::start();

  // Create a rotating file sink
  auto rotating_file_sink =
      quill::Frontend::create_or_get_sink<quill::RotatingFileSink>(
          filename, []() {
            quill::RotatingFileSinkConfig cfg;
            cfg.set_rotation_on_creation(true);
            cfg.set_filename_append_option(
                quill::FilenameAppendOption::StartDateTime);
            cfg.set_rotation_time_daily("22:30");
            cfg.set_rotation_max_file_size(1024 * 1024);
            cfg.set_max_backup_files(7);
            return cfg;
          }());

  // Create the logger with default log level: info
  l_ = quill::Frontend::create_or_get_logger(loggerName, rotating_file_sink);
}

Quill_logger::~Quill_logger() {}

// set_log_level()
void Quill_logger::set_log_level(quill::LogLevel level) {
  l_->set_log_level(level);
}

// flush_log()
void Quill_logger::flush_log() { l_->flush_log(); }

// set_immediate_flush()
void Quill_logger::set_immediate_flush() { l_->set_immediate_flush(); }
