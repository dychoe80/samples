#pragma once

#include <string>
#include <memory>  // std::unique_ptr
#include <cstdarg>

// This implements can be replaced by C++11 std::snprintf
inline std::string format(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    const unsigned int len = _vscprintf(fmt, args) + 1;
    std::unique_ptr<char[]> buf(new char[len]);
    vsprintf_s(buf.get(), len, fmt, args);

    va_end(args);
    return buf.get();
}

inline std::wstring format(const wchar_t* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    const unsigned int len = _vscwprintf(fmt, args) + 1;
    std::unique_ptr<wchar_t[]> buf(new wchar_t[len]);
    vswprintf_s(buf.get(), len, fmt, args);

    va_end(args);
    return buf.get();
}
