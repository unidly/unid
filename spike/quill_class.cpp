// gcc14-g++ quill_class.cpp
// -I/home/msellers/.conan2/p/quill4d4d9a8b27a55/p/include/ -o
// bin/quill_class
#include "quill/Backend.h"
#include "quill/Frontend.h"
#include "quill/LogMacros.h"
#include "quill/Logger.h"
#include "quill/sinks/FileSink.h"
#include "quill/sinks/RotatingFileSink.h"
#include <memory>
#include <string>

class Quill_logger {
public:
  Quill_logger(const std::string &loggerName, const std::string &filename) {
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

    // Create the logger
    l_ = quill::Frontend::create_or_get_logger(loggerName, rotating_file_sink);
    l_->set_log_level(quill::LogLevel::Debug);
  }

  /**
   * @brief Sets the logging level
   *
   * @param level The log level
   */
  void set_log_level(quill::LogLevel level) { l_->set_log_level(level); }

  /**
   * @brief Flushes the log
   *
   * This function blocks the calling thread until the backend has finished
   * processing and writing all pending log entries.
   */
  void flush_log() { l_->flush_log(); }

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
  void set_immediate_flush() { l_->set_immediate_flush(); }

  /**
   * @brief Member logging functions
   *
   * These functions allow the logging of messages at the levels indicated
   * by the function name.
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

int main() {
  std::unique_ptr<Quill_logger> logger =
      std::make_unique<Quill_logger>("app_logger", "app.log");

  logger->info("Application started");
  logger->error("An error occurred");

  return 0;
}
