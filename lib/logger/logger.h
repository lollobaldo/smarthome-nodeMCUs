enum LogLevel { INFO, LOG, WARN, ERROR };

namespace logger {
    boolean log(LogLevel logLevel, const char* topic, const char* message);
    boolean log(const char* topic, const char* message);
    boolean log(const char* topic, const char* message, boolean retained);
}
