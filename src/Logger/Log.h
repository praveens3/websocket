#ifndef MYLIB_H
#define MYLIB_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <memory>
#include <string>

#define logSection "__log__"
// Initialize a logger (could be called in your library initialization code)
void init_logger_console();

// Initialize a file logger
void initLog(const std::string& sectionName, const std::string& logFileName = "log");

//print test log content
void print_test_logs();

// Logging macros
#define LOG_DEBUG spdlog::get(logSection)->debug
#define LOG_INFO spdlog::get(logSection)->info
#define LOG_WARN spdlog::get(logSection)->warn
#define LOG_ERROR spdlog::get(logSection)->error
#define LOG_CRITICAL spdlog::get(logSection)->critical

//#define LOG_DEBUG spdlog::get("default")->debug
//#define LOG_INFO spdlog::get("default")->info
//#define LOG_WARN spdlog::get("default")->warn
//#define LOG_ERROR spdlog::get("default")->error
//#define LOG_CRITICAL spdlog::get("default")->critical

/*#define LOG_INFO(msg, ...) spdlog::get(logger_name)->info(msg, __VA_ARGS__)
#define LOG_DEBUG(msg, ...) spdlog::get(logSection)->debug(msg, __VA_ARGS__)
#define LOG_WARN(msg, ...) spdlog::get(logSection)->warn(msg, __VA_ARGS__)
#define LOG_ERROR(msg, ...) spdlog::get(logSection)->error(msg, __VA_ARGS__)
#define LOG_CRITICAL(msg, ...) spdlog::get(logSection)->critical(msg, __VA_ARGS__)
*/

// Function to set the logging level
void set_log_level(spdlog::level::level_enum level);

#endif // MYLIB_H