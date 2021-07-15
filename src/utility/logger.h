#ifndef BASICGAMEENGINE_LOGGER_H
#define BASICGAMEENGINE_LOGGER_H


#include <string>
#include <iostream>

enum loggerType {
    INFO,           // white
    INFO_IMPORTANT, // green
    OUTPUT,         // blue
    WARNING,        // yellow
    ERROR           // red
};

class abstractLogger {
public:
    virtual void logInfo(const std::string& source, const std::string& message) = 0;
    virtual void logInfoImportant(const std::string& source, const std::string& message) = 0;
    virtual void logOutput(const std::string& source, const std::string& message) = 0;
    virtual void logWarning(const std::string& source, const std::string& message) = 0;
    virtual void logError(const std::string& source, const std::string& message) = 0;
    static std::string formatMessage(loggerType type, const std::string& source, const std::string& message) {
        // NOTE: This formatting is pretty much hardcoded, try to stick to it
        switch (type) {
            case INFO:
                return INFO_PREFIX + "[" + source + "] " + message;
            case INFO_IMPORTANT:
                return INFO_IMPORTANT_PREFIX + "[" + source + "] " + message;
            case OUTPUT:
                return OUTPUT_PREFIX + "[" + source + "] " + message;
            case WARNING:
                return WARNING_PREFIX + "[" + source + "] " + message;
            case ERROR:
                return ERROR_PREFIX + "[" + source + "] " + message;
        }
        return "";
    }
    static inline std::string INFO_PREFIX = "[*]";
    static inline std::string INFO_IMPORTANT_PREFIX = "[!]";
    static inline std::string OUTPUT_PREFIX = "[O]";
    static inline std::string WARNING_PREFIX = "[W]";
    static inline std::string ERROR_PREFIX = "[E]";
};

class logger : public abstractLogger {
public:
    void logInfo(const std::string& source, const std::string& message) override {
#if DEBUG
        std::cout << formatMessage(INFO, source, message) << std::endl;
#endif
    }

    void logInfoImportant(const std::string& source, const std::string& message) override {
#if DEBUG
        std::cout << "\x1B[32m" << formatMessage(INFO_IMPORTANT, source, message) << "\033[0m" << std::endl;
#endif
    }

    void logOutput(const std::string& source, const std::string& message) override {
#if DEBUG
        std::cout << "\x1B[34m" << formatMessage(OUTPUT, source, message) << "\033[0m" << std::endl;
#endif
    }

    void logWarning(const std::string& source, const std::string& message) override {
#if DEBUG
        std::cout << "\x1B[33m" << formatMessage(WARNING, source, message) << "\033[0m" << std::endl;
#endif
    }

    void logError(const std::string& source, const std::string& message) override {
#if DEBUG
        std::cout << "\x1B[31m" << formatMessage(ERROR, source, message) << "\033[0m" << std::endl;
#endif
    }
};


#endif //BASICGAMEENGINE_LOGGER_H
