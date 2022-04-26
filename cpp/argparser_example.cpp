#include "argparser.h"
#include <iostream>

int main(int argc, char** argv)
{
    auto argParser = argparser{ argc, argv };

    auto const help = argParser.checkCmdOption("-h");
    if (help)
    {
        std::cerr << "this is a example of argparser" << std::endl;
        return 0;
    }
  
    auto const device = argParser.getCmdOption("-d", 0);
    auto const sampleRate = argParser.getCmdOption("-s", 16000.0);
    auto const blockSize = argParser.getCmdOption("-b", 8000);
    auto const model = argParser.getCmdOption("-m", "model");

    std::cout << "device: " << device << "\n";
    std::cout << "sample rate: " << sampleRate << "\n";
    std::cout << "block size: " << blockSize << "\n";
    std::cout << "model: " << model << std::endl;

    return 0;
}
