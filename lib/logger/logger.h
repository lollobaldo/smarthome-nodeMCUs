enum LogLevel { INFO, LOG, WARN, ERROR };

// namespace logger {
//     bool log(LogLevel logLevel, const char* topic, const char* message, bool retained);
//     bool log(LogLevel logLevel, const char* message);
//     bool log(const char* message);
//     bool log(const char* topic, const char* message, bool retained);
// }

class Logger {
    public:
        Logger(const char* prefix);
        void info(const char* message);
        void log(const char* message);
        void warn(const char* message);
        void error(const char* message);
        void log(LogLevel logLevel, const char* message, bool retained);
        void log(LogLevel logLevel, const char* message);
        void log(const char* message, bool retained);
    private:
        const char* topic;
        const char* prefix;
};
