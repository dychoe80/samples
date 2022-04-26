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
            m_tokens.push_back(argv[i]);
    }

    template <typename T>
    T getCmdOption(const std::string& option, T defaultValue) const
    {
        auto itr = std::find(m_tokens.begin(), m_tokens.end(), option);
        if (itr != m_tokens.end() && ++itr != m_tokens.end())
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
        auto itr = std::find(m_tokens.begin(), m_tokens.end(), option);
        if (itr != m_tokens.end() && ++itr != m_tokens.end())
        {
            return *itr;
        }
        return defaultValue;
    }

    bool checkCmdOption(const std::string& option) const
    {
        return std::find(m_tokens.begin(), m_tokens.end(), option) != m_tokens.end();
    }

private:
    std::vector<std::string> m_tokens;
};
