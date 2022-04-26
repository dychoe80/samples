#pragma once

#include <vector>
#include <sstream>
#include <string>
#include <utility>

class argparser
{
public:
    argparser(int argc, char** argv)
    {
        for (int i = 1; i < argc; ++i)
            tokens_.push_back(argv[i]);
    }

    template <typename T>
    T getCmdOption(const std::string& option, T defaultValue) const
    {
        auto itr = std::find(tokens_.begin(), tokens_.end(), option);
        if (itr != tokens_.end() && ++itr != tokens_.end())
        {
            std::stringstream ss;
            ss << *itr;
            T val{};
            ss >> val;
            return val;
        }
        return defaultValue;
    }

    std::string getCmdOption(const std::string& option, const char* defaultValue) const
    {
        auto itr = std::find(tokens_.begin(), tokens_.end(), option);
        if (itr != tokens_.end() && ++itr != tokens_.end())
        {
            return *itr;
        }
        return defaultValue;
    }

    bool checkCmdOption(const std::string& option) const
    {
        return std::find(tokens_.begin(), m_tokens.end(), option) != tokens_.end();
    }

private:
    std::vector<std::string> tokens_;
};
