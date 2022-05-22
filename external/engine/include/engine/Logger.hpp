#pragma once
#include "DebugColors.h"
#include <iostream>
#include "engine/Core.h"

#define LOG_I(message) Logger::LogI(message, __FILE__, __LINE__)
#define LOG_E(message) { Logger::LogE(message, __FILE__, __LINE__); BREAK(); }
#define LOG_C(message) { Logger::LogC(message, __FILE__, __LINE__); BREAK(); }
#define LOG_W(message) Logger::LogW(message, __FILE__, __LINE__)

enum class LogType: int {
    Info,
    Error,
    Critical,
    Warning
};

inline std::string LogTypeToString(const LogType& t)
{
    switch (t)
    {
    case LogType::Info:
        return DC_INFO;
    case LogType::Error:
        return DC_ERROR;
    case LogType::Warning:
        return DC_WARNING;
    case LogType::Critical:
        return "[" DC_RED "CRITICAL ERROR" DC_DEFAULT "]";
    default:
        return "[unknown]";
    }
}

// TODO: Logger should be able to output logs to a file, which will then contain: date, time, file, line, thread and message.
//       etc.
class Logger
{
public:
    static void Log(const LogType& type, const std::string& message, const std::string& file, const int& line)
    {
        std::cout << LogTypeToString(type) << " " << file << ":" << line << ": " << message << std::endl;
    }
    static void LogI(const std::string& message, const std::string& file, const int& line) { Log(LogType::Info, message, file, line); }
    static void LogE(const std::string& message, const std::string& file, const int& line) { Log(LogType::Error, message, file, line); }
    static void LogC(const std::string& message, const std::string& file, const int& line) { Log(LogType::Critical, message, file, line); }
    static void LogW(const std::string& message, const std::string& file, const int& line) { Log(LogType::Warning, message, file, line); }

private:
    Logger() {};
};