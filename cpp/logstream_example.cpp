#include "logstream.h"

int main()
{
    auto ls = logstream{"logfile.txt", 1024};

    for (int i = 0; i < 100; ++i)
        ls << "this is a test: line " << i << std::endl;

    return 0;
}