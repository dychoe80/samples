// This file is subject to the terms and conditions defined in
// file 'LICENSE', which is part of this source code package.

// Test various methods to measure execution time.
// Here, we measure elapsed times of matrix multiplication (dot product)

#include "time_example.h"
#include "matrix.h"

#include <vector>
#include <random>
#include <iostream>

#include <Windows.h>
#pragma comment(lib, "ntdll.lib")

extern "C" NTSYSAPI NTSTATUS NTAPI NtQueryTimerResolution(OUT PULONG MaximumResolution, OUT PULONG MinimumResolution, OUT PULONG CurrentResolution);
extern "C" NTSYSAPI NTSTATUS NTAPI NtSetTimerResolution(ULONG DesiredResolution, BOOLEAN SetResolution, PULONG CurrentResolution);

int main(int argc, char* argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <time method index> <number of rows> <number of columns>" << std::endl;
        return EXIT_FAILURE;
    }
    int const timeMethod = std::atoi(argv[1]);
    int const M = std::atoi(argv[2]);
    int const N = std::atoi(argv[3]);

    // (MxN) * (NxM) = (MxM)
    Dim const aDim{ M, N }, bDim{ N, M }, cDim{ M, M };
    std::vector<float> A(M * N), B(N * M), C(M * M);

    // Initialize matrix as random
    std::default_random_engine rd;
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (auto& a : A)
        a = dist(rd);
    for (auto& b : B)
        b = dist(rd);    

    // Check system timer resolution
    ULONG timeResMax = 0, timeResMin = 0, timeResCur = 0;
    NtQueryTimerResolution(&timeResMax, &timeResMin, &timeResCur);
    std::cout << "Timer resolution [usec]: max " << timeResMax * 0.1 << ", min " << timeResMin * 0.1 << ", current " << timeResCur * 0.1 << std::endl;

    // Check system time adjustment
    DWORD timeAdjust = 0, timeIncre = 0;
    BOOL timeAdjustDisabled = FALSE;
    GetSystemTimeAdjustment(&timeAdjust, &timeIncre, &timeAdjustDisabled);
    std::cout << "Time adjustment [usec]: adjust " << timeAdjust * 0.1 << ", increment " << timeIncre * 0.1 << ", disabled " << timeAdjustDisabled << std::endl;

    //
    // Run test
    //
    auto target = [&A, &aDim, &B, &bDim, &C]() { dotProd(&A[0], aDim, &B[0], bDim, &C[0]); };
    
    switch (timeMethod)
    {
    case 0:
        doExample__chrono_steady_clock(target);
        break;
    case 1:
        doExample__chrono_high_resolution(target);
        break;
    case 2:
        doExample__chrono_system_clock(target);
        break;
    case 3:
        doExample__QueryPerformanceCounter(target);
        break;
    case 4:
        doExample__clock(target);
        break;
    case 5:
        doExample__ippGetCpuClocks(target);
        break;
    case 6:
        doExample__GetProcessTimes(target);
        break;
    case 7:
        doExample__GetThreadTimes(target);
        break;
    case 8:
        doExample__rdtsc(target);
        break;
    case 9:
        doExample__time(target);
        break;
    case 10:
        doExample__timeGetTime(target);
        break;
    case 11:
        doExample__getTickCount64(target);
        break;
    case 12:
        doExample__QueryInterruptTime(target);
        break;
    case 13:
        doExample__QueryInterruptTimePrecise(target);
        break;
    case 14:
        doExample__GetLocalTime(target);
        break;
    case 15:
        doExample__GetSystemTime(target);
        break;
    case 16:
        doExample__GetSystemTimeAsFileTime(target);
        break;
    case 17:
        doExample__GetSystemTimePreciseAsFileTime(target);
        break;
    }

    //printMatrix(&C[0], cDim);

    return EXIT_SUCCESS;
}
