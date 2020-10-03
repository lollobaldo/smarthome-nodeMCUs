enum LogLevel { INFO, LOG, WARN, ERROR };

namespace logger {
    bool log(LogLevel logLevel, const char* topic, const char* message);
    bool log(const char* topic, const char* message);
    bool log(const char* topic, const char* message, bool retained);
}
