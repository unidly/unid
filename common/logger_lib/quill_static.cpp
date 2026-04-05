/**
 * @file
 * @brief Quill static library implementatoin
 */
#include "quill_static.hpp"

#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/Logger.h"
#include "quill/sinks/RotatingFileSink.h"

// Define a global variable for a logger to avoid looking up the logger each
// time. Additional global variables can be defined for additional loggers if
// needed.
quill::Logger *global_logger_a; // General purpose logger

/**
 * @brief Setup quill filesink and create a global logger
 *
 * This library implementation follows the Quill Library recommended usage
 * pattern for usage. By creating a pre-compiled library, project compile time
 * is reduced, and management of loggers and sinks is centralized outside of
 * the project source code.
 *
 * This function is called prior to any logging operation.
 *
 * @note A main entry point calls this function. Only the main function
 * accesses the global logger pointer. The logger pointer is passed as a
 * dependency into classes that require access to the logger. The global
 * pointer should not be accessed outside of main!
 *
 * @param log_file The name and filepath of the log file
 */
void setup_quill(char const *log_file) {
  // Start the backend thread
  quill::Backend::start();

  // Setup rotating file sink
  auto rotating_file_sink =
      quill::Frontend::create_or_get_sink<quill::FileSink>(
          log_file,
          []() {
            quill::RotatingFileSinkConfig cfg;
            cfg.set_rotation_on_creation(true);
            cfg.set_filename_append_option(
                quill::FilenameAppendOption::StartDateTime);
            cfg.set_rotation_time_daily("22:30");
            cfg.set_rotation_max_file_size(1024 * 1024);
            cfg.set_max_backup_files(7);
            return cfg;
          }(),
          quill::FileEventNotifier{});

  // Create and store the logger
  global_logger_a = quill::Frontend::create_or_get_logger(
      "root", std::move(rotating_file_sink),
      quill::PatternFormatterOptions{
          "%(time) [%(thread_id)] %(short_source_location:<28) "
          "LOG_%(log_level:<9) %(logger:<12) %(message)",
          "%H:%M:%S.%Qns", quill::Timezone::GmtTime});
}
