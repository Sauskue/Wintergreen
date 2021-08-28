#pragma once
#include <Windows.h>
#include <sstream>
#include <time.h>

#define LOG Logger::Log
#define MAX_SIZE 26

class Logger
{
public:
    Logger() = delete;
    ~Logger() = delete;
    Logger(const Logger& other) = delete;
    const Logger& operator=(const Logger& other) = delete;

    enum class LogLevel
    {
        None,			//Used to turn off all logging
        Trace,			//Used to track execution stages
        Debug,			//Used for detailed developer information
        Info,			//Used for reporting system diagnostics and state changes
        Warning,        //Used for questionining an action
        Error,			//Used for recoverable error conditions
        Critical,		//Used for errors that force the appication to terminate
        Default = Info	//Used if logging without setting a level
    };
private:
    static LogLevel current_level;
    static LogLevel min_level;
    static HANDLE console_handle;

    static bool initialized;
    static bool multi_params;
    static int count;

    static std::stringstream combined_text;
    static const char* label;
public:
    static void Init();
    static void Cleanup();
    static void SetMinLevel(LogLevel);

    template<typename T>
    static void Log(T t)
    {
        combined_text << t;
        if (multi_params)
            count--;
        else
            PrintLog();
    }

    template<>
    static void Log<LogLevel>(LogLevel level)
    {
        SetLevel(level);
        if (multi_params)
            count--;
    }

    template<typename T, typename...U>
    static void Log(T first, U... args)
    {
        multi_params = true;
        count++;
        Log(first);
        Log(args...);
        if (count == -1)
            PrintLog();
    }

private:
    static void SetLevel(LogLevel level);
    static void PrintLog();

};
#define LogLevel Logger::LogLevel