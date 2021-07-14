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
        switch (type) {
            case INFO:
                return "[*][" + source + "] " + message;
            case INFO_IMPORTANT:
                return "[!][" + source + "] " + message;
            case OUTPUT:
                return "[O][" + source + "] " + message;
            case WARNING:
                return "[W][" + source + "] " + message;
            case ERROR:
                return "[E][" + source + "] " + message;
        }
        return "";
    }
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
