enum LogLevel { INFO, LOG, WARN, ERROR };

class Logger {
    public:
        Logger(const char* prefix);
        void info(const char* message);
        void log(const char* message);
        void warn(const char* message);
        void error(const char* message);
        void log(LogLevel logLevel, const char* message);
    private:
        const char* prefix;
};
