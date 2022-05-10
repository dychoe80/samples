#pragma once

#include <string>
#include <ostream>
#include <sstream>


class logstream : public std::ostream
{
public:
    logstream();
    logstream(std::string const& logFilePath, std::size_t logFileMaxSize);
    virtual ~logstream();

    void setLogFilePath(std::string const& logFilePath);
    std::string getLogFilePath() const;

    void setLogFileMaxSize(std::size_t logFileMaxSize);
    std::size_t getLogFileMaxSize() const;
};
