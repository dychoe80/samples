// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

#pragma once

#include "ippcore.h"  // ippGetCpuClocks
#pragma comment(lib, "ippcoremt.lib")  // static linking to IPP core

#include <chrono>  // std::chrono::steady_clock
#include <iostream>

#include <ctime>  // std::clock, std::time

#include <intrin.h>  // __rdtsc

#include <Windows.h>  // QueryPerformanceCounter, GetTickCount64, timeGetTime
#pragma comment(lib, "Winmm.lib")  // timeGetTime

#include <realtimeapiset.h>
#pragma comment(lib, "Mincore.lib")  // QueryInterruptTime, QueryInterruptTimePrecise


// std::chrono::steady_clock
template <typename Functor>
void doExample__chrono_steady_clock(Functor&& func)
{
    std::chrono::steady_clock::time_point const begin = std::chrono::steady_clock::now();

    func();

    std::chrono::steady_clock::time_point const end = std::chrono::steady_clock::now();

    std::cout << "Time difference [usec]: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
}


// std::chrono::high_resolution_clock
template <typename Functor>
void doExample__chrono_high_resolution(Functor&& func)
{
    std::chrono::high_resolution_clock::time_point const begin = std::chrono::high_resolution_clock::now();

    func();

    std::chrono::high_resolution_clock::time_point const end = std::chrono::high_resolution_clock::now();

    std::cout << "Time difference [usec]: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
}


// std::chrono::system_clock
template <typename Functor>
void doExample__chrono_system_clock(Functor&& func)
{
    std::chrono::system_clock::time_point const begin = std::chrono::system_clock::now();

    func();

    std::chrono::system_clock::time_point const end = std::chrono::system_clock::now();

    std::cout << "Time difference [usec]: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << std::endl;
}


// QueryPerformanceCounter()
template <typename Functor>
void doExample__QueryPerformanceCounter(Functor&& func)
{
    LARGE_INTEGER begin;
    QueryPerformanceFrequency(&begin);
    double const pcFreqMhz = begin.QuadPart / 1000000.0;
    QueryPerformanceCounter(&begin);

    func();

    LARGE_INTEGER end;
    QueryPerformanceCounter(&end);

    std::cout << "Time difference [usec]: " << (end.QuadPart - begin.QuadPart) / pcFreqMhz << std::endl;
}


// std::clock()
template <typename Functor>
void doExample__clock(Functor&& func)
{
    std::clock_t const beginClk = std::clock();

    func();

    std::clock_t const endClk = std::clock();

    std::cout << "Time difference [sec]: " << (endClk - beginClk) * 1.0 / CLOCKS_PER_SEC << std::endl;
}


// ippGetCpuClock()
template <typename Functor>
void doExample__ippGetCpuClocks(Functor&& func)
{
    int cpuFreqMhz = 0;
    ippGetCpuFreqMhz(&cpuFreqMhz);
    Ipp64u const beginClk = ippGetCpuClocks();

    func();

    Ipp64u const endClk = ippGetCpuClocks();

    std::cout << "Time difference [usec]: " << (endClk - beginClk) * 1.0 / cpuFreqMhz << std::endl;
}


// GetProcessTimes()
template <typename Functor>
void doExample__GetProcessTimes(Functor&& func)
{
    FILETIME createTime = {}, exitTime = {}, kernelTime = {}, userTime = {};
    HANDLE const thisProc = GetCurrentProcess();

    GetProcessTimes(thisProc, &createTime, &exitTime, &kernelTime, &userTime);
    long long const kernelBegin = kernelTime.dwLowDateTime | (static_cast<long long>(kernelTime.dwHighDateTime) << 32);
    long long const userBegin = userTime.dwLowDateTime | (static_cast<long long>(userTime.dwHighDateTime) << 32);

    func();

    GetProcessTimes(thisProc, &createTime, &exitTime, &kernelTime, &userTime);
    long long const kernelEnd = kernelTime.dwLowDateTime | (static_cast<long long>(kernelTime.dwHighDateTime) << 32);
    long long const userEnd = userTime.dwLowDateTime | (static_cast<long long>(userTime.dwHighDateTime) << 32);

    std::cout << "Kerne lTime difference [usec]: " << (kernelEnd - kernelBegin) * 0.1 << std::endl;
    std::cout << "User Time difference [usec]: " << (userEnd - userBegin) * 0.1 << std::endl;
}


// GetThreadTimes()
template <typename Functor>
void doExample__GetThreadTimes(Functor&& func)
{
    FILETIME createTime = {}, exitTime = {}, kernelTime = {}, userTime = {};
    HANDLE const thisThread = GetCurrentThread();

    GetThreadTimes(thisThread, &createTime, &exitTime, &kernelTime, &userTime);
    long long const kernelBegin = kernelTime.dwLowDateTime | (static_cast<long long>(kernelTime.dwHighDateTime) << 32);
    long long const userBegin = userTime.dwLowDateTime | (static_cast<long long>(userTime.dwHighDateTime) << 32);

    func();

    GetThreadTimes(thisThread, &createTime, &exitTime, &kernelTime, &userTime);
    long long const kernelEnd = kernelTime.dwLowDateTime | (static_cast<long long>(kernelTime.dwHighDateTime) << 32);
    long long const userEnd = userTime.dwLowDateTime | (static_cast<long long>(userTime.dwHighDateTime) << 32);

    std::cout << "Kerne lTime difference [usec]: " << (kernelEnd - kernelBegin) * 0.1 << std::endl;
    std::cout << "User Time difference [usec]: " << (userEnd - userBegin) * 0.1 << std::endl;
}


// __rdtsc()
template <typename Functor>
void doExample__rdtsc(Functor&& func)
{
    unsigned __int64 const beginClk = __rdtsc();

    func();

    unsigned __int64 const endClk = __rdtsc();

    std::cout << "Time difference [clk]: " << (endClk - beginClk) << std::endl;
}

// std::time()
template <typename Functor>
void doExample__time(Functor&& func)
{
    std::time_t beginSec = {};
    std::time(&beginSec);

    func();

    std::time_t endSec = {};
    std::time(&endSec);

    std::cout << "Time difference [sec]: " << std::difftime(endSec, beginSec) << std::endl;
}


// timeGetTime()
template <typename Functor>
void doExample__timeGetTime(Functor&& func)
{
    DWORD const beginMsec = timeGetTime();

    func();

    DWORD const endMsec = timeGetTime();

    std::cout << "elapsed time [msec]: " << (endMsec - beginMsec) << std::endl;
}


// getTickCount64()
template <typename Functor>
void doExample__getTickCount64(Functor&& func)
{
    ULONGLONG const beginMsec = GetTickCount64();

    func();

    ULONGLONG const endMsec = GetTickCount64();

    std::cout << "Time difference [msec]: " << (endMsec - beginMsec)  << std::endl;
}


// QueryInterruptTime()
template <typename Functor>
void doExample__QueryInterruptTime(Functor&& func)
{
    ULONGLONG begin = 0;
    QueryInterruptTime(&begin);  // or QueryUnbiasedInterruptTime()

    func();

    ULONGLONG end = 0;
    QueryInterruptTime(&end);

    std::cout << "Time difference [usec]: " << (end - begin) * 0.1 << std::endl;
}


// QueryInterruptTimePrecise()
template <typename Functor>
void doExample__QueryInterruptTimePrecise(Functor&& func)
{
    ULONGLONG begin = 0;
    QueryInterruptTimePrecise(&begin);  // QueryUnbiasedInterruptTimePrecise()

    func();

    ULONGLONG end = 0;
    QueryInterruptTimePrecise(&end);

    std::cout << "Time difference [usec]: " << (end - begin) * 0.1 << std::endl;
}


// GetLocalTime()
template <typename Functor>
void doExample__GetLocalTime(Functor&& func)
{
    SYSTEMTIME beginSysTime = {};
    GetLocalTime(&beginSysTime);

    func();

    SYSTEMTIME endSysTime = {};
    GetLocalTime(&endSysTime);

    // Convert system time to file time
    FILETIME beginFileTime = {};
    SystemTimeToFileTime(&beginSysTime, &beginFileTime);

    FILETIME endFileTime = {};
    SystemTimeToFileTime(&endSysTime, &endFileTime);

    long long const beginTime = beginFileTime.dwLowDateTime | (static_cast<long long>(beginFileTime.dwHighDateTime) << 32);
    long long const endTime = endFileTime.dwLowDateTime | (static_cast<long long>(endFileTime.dwHighDateTime) << 32);

    std::cout << "Time difference [usec]: " << (endTime - beginTime) * 0.1 << std::endl;
}


// GetSystemTime()
template <typename Functor>
void doExample__GetSystemTime(Functor&& func)
{
    SYSTEMTIME beginSysTime = {};
    GetSystemTime(&beginSysTime);

    func();

    SYSTEMTIME endSysTime = {};
    GetSystemTime(&endSysTime);

    // Convert system time to file time
    FILETIME beginFileTime = {};
    SystemTimeToFileTime(&beginSysTime, &beginFileTime);

    FILETIME endFileTime = {};
    SystemTimeToFileTime(&endSysTime, &endFileTime);

    long long const beginTime = beginFileTime.dwLowDateTime | (static_cast<long long>(beginFileTime.dwHighDateTime) << 32);
    long long const endTime = endFileTime.dwLowDateTime | (static_cast<long long>(endFileTime.dwHighDateTime) << 32);

    std::cout << "Time difference [usec]: " << (endTime - beginTime) * 0.1 << std::endl;
}


// GetSystemTimeAsFileTime()
template <typename Functor>
void doExample__GetSystemTimeAsFileTime(Functor&& func)
{
    FILETIME begin = {};
    GetSystemTimeAsFileTime(&begin);

    func();

    FILETIME end = {};
    GetSystemTimeAsFileTime(&end);

    long long const beginTime = begin.dwLowDateTime | (static_cast<long long>(begin.dwHighDateTime) << 32);
    long long const endTime = end.dwLowDateTime | (static_cast<long long>(end.dwHighDateTime) << 32);

    std::cout << "Time difference [usec]: " << (endTime - beginTime) * 0.1 << std::endl;
}


// GetSystemTimePreciseAsFileTime()
template <typename Functor>
void doExample__GetSystemTimePreciseAsFileTime(Functor&& func)
{
    FILETIME begin = {};
    GetSystemTimePreciseAsFileTime(&begin);

    func();

    FILETIME end = {};
    GetSystemTimePreciseAsFileTime(&end);

    long long const beginTime = begin.dwLowDateTime | (static_cast<long long>(begin.dwHighDateTime) << 32);
    long long const endTime = end.dwLowDateTime | (static_cast<long long>(end.dwHighDateTime) << 32);

    std::cout << "Time difference [usec]: " << (endTime - beginTime) * 0.1 << std::endl;
}
