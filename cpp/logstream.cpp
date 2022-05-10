#include "logstream.h"

#include <sstream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <Windows.h>


class logbuf : public std::stringbuf
{
    friend class logstream;

public:
    logbuf() = default;
    virtual ~logbuf();

    virtual int sync() override;

private:
    std::string logFilePath_;
    std::size_t logFileMaxSize_ = 10485760;  // Max 10 MB by default
    std::size_t messageCount_ = 0;
};


logbuf::~logbuf()
{
    if (!str().empty())
        pubsync();
}


int logbuf::sync()
{
    // Use OutputDebugString() in case that the log file is not specified
    if (logFilePath_.empty())
    {
        OutputDebugStringA(str().c_str());
        return 0;
    }

    // date/time is followed by message 
    auto const now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    struct tm buf;
    auto const err = localtime_s(&buf, &now);
    if (err != 0)
        return -1;

    auto dateTime = std::ostringstream{};
    dateTime << std::put_time(&buf, "%Y-%m-%d %H:%M:%S");

    auto logFile = std::ofstream{ logFilePath_, std::ios::app };

    // Roll the log file to avoid accumulation
    if ((++messageCount_ % 100) == 0) // Check file size every 100 messages to reduce overhead
    {
        // Get the log file size
        logFile.seekp(0, std::ios::end);
        auto const logFileSize = static_cast<decltype(logFileMaxSize_)>(std::streamoff{ logFile.tellp() });

        // Roll the log file when its size exceeds the max specified
        if (logFileSize > logFileMaxSize_)
        {
            logFile.close();
            auto const backupFilePath = logFilePath_ + ".bak";
            if (::MoveFileExA(logFilePath_.c_str(), backupFilePath.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
                logFile.open(logFilePath_, std::ios::out | std::ios::app);
        }
    }

    // Write the message to the file
    logFile << dateTime.str() << "; " << str();
    logFile.close();

    str("");

    return 0;
}


logstream::logstream()
    : std::ostream{ new logbuf }
{
}


logstream::logstream(std::string const& logFilePath, std::size_t logFileMaxSize)
    : logstream{}
{
    setLogFilePath(logFilePath);
    setLogFileMaxSize(logFileMaxSize);
}


logstream::~logstream()
{
    delete rdbuf();
}


void logstream::setLogFilePath(std::string const& logFilePath)
{
    static_cast<logbuf*>(rdbuf())->logFilePath_ = logFilePath;
}


std::string logstream::getLogFilePath() const
{
    return static_cast<logbuf*>(rdbuf())->logFilePath_;
}


void logstream::setLogFileMaxSize(std::size_t logFileMaxSize)
{
    static_cast<logbuf*>(rdbuf())->logFileMaxSize_ = logFileMaxSize;
}


std::size_t logstream::getLogFileMaxSize() const
{
    return static_cast<logbuf*>(rdbuf())->logFileMaxSize_;
}
