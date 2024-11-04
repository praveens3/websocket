#include "Log.h"
#include "spdlog/sinks/basic_file_sink.h"

struct Initializer {
    Initializer() {
        initLog(logSection);
        LOG_INFO("---------- Logger initialized ---------");
    }
    ~Initializer() {
        LOG_INFO("---------- Logger ends ----------\n");
    }
};

namespace {
	static Initializer i;
}
// Initialize a logger (could be called in your library initialization code)
void init_logger_console() {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern("[%Y-%m-%d %H:%M:%S] [%^%l%$] %v");
    const std::string logger_name = "console-logger";
    auto logger = std::make_shared<spdlog::logger>(logger_name.c_str(), console_sink);
    spdlog::register_logger(logger);
}

// Initialize a file logger
void initLog(const std::string& sectionName, const std::string& logFileName) {
    try {
        const std::string filePath = "logs/" + logFileName + ".txt";
        auto file_logger = spdlog::basic_logger_mt(sectionName, filePath);
        //spdlog::set_default_logger(file_logger);
        file_logger->set_level(spdlog::level::info); // Set the desired log level
        if (sectionName == "__log__") {
            file_logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%l] %v [t %t]");
        }
        else {
            file_logger->set_pattern("%Y-%m-%d %H:%M:%S.%e [%l] [%n] %v [t %t] ");
        }
		file_logger->flush_on(spdlog::level::info);
    }
    catch (const spdlog::spdlog_ex& ex) {
        spdlog::warn("{0} initialization failed: {1}", sectionName, ex.what());
    }
}

void set_log_level(spdlog::level::level_enum level) {
    spdlog::set_level(level);
    spdlog::info("log level changed to {0}", (int)level);
}

void print_test_logs()
{
    spdlog::set_level(spdlog::level::info); // Set global log level to debug

    spdlog::info("Welcome to spdlog!");
    spdlog::error("Some error message with arg: {}", 1);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:<30}", "left aligned");

    spdlog::debug("This message should not be displayed..");
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    spdlog::debug("This message should be displayed..\n\n");
}