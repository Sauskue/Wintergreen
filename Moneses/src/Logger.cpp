#include "Logger.h"

Logger::LogLevel Logger::current_level;
Logger::LogLevel Logger::min_level;
HANDLE Logger::console_handle;
bool Logger::initialized = false;
bool Logger::multi_params;
int Logger::count;
std::stringstream Logger::combined_text;
const char* Logger::label;

void Logger::Init()
{
    if (initialized)
        return;
    console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetLevel(LogLevel::Default);
    SetMinLevel(LogLevel::None);
    initialized = true;
    Log("Logger intialized");
}

void Logger::Cleanup()
{
    SetLevel(LogLevel::None);
}

void Logger::PrintLog()
{
    if (!initialized)
    {
        return;
    }
    if (current_level != LogLevel::None && current_level >= min_level)
    {
        time_t ltime;
        char buf[MAX_SIZE];
        time(&ltime);
        ctime_s(buf, MAX_SIZE, &ltime);
        printf(label);
        printf_s("[%.24s]: ", buf);
        printf(combined_text.str().c_str());
        printf("\n");
    }

    combined_text.str("");
    multi_params = false;
    count = 0;
    SetLevel(LogLevel::Default);
}

void Logger::SetLevel(LogLevel level)
{
    current_level = level;
    switch (current_level)
    {
        case LogLevel::None:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            label = "";
            break;
        }
        case LogLevel::Trace:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            label = "[TRACE]";
            break;
        }
        case LogLevel::Debug:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            label = "[DEBUG]";
            break;
        }
        case LogLevel::Info:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            label = "[INFO]";
            break;
        }
        case LogLevel::Warning:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            label = "[WARNING]";
            break;
        }
        case LogLevel::Error:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_INTENSITY);
            label = "[ERROR]";
            break;
        }
        case LogLevel::Critical:
        {
            SetConsoleTextAttribute(console_handle, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_INTENSITY);
            label = "[CRITICAL]";
            break;
        }
    }
}

void Logger::SetMinLevel(LogLevel level)
{
    min_level = level;
}