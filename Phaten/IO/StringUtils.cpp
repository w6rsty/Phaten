#include "StringUtils.hpp"

#include "Core/Assert.hpp"

namespace Pt {

std::string FormatString(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
}

std::vector<std::string> Split(std::string_view str, char delimiter)
{
    std::vector<std::string> ret;
    size_t start = 0;
    size_t end = 0;
    while ((end = str.find(delimiter, start)) != std::string::npos)
    {
        ret.push_back(std::string(str.substr(start, end - start)));
        start = end + 1;
    }
    ret.push_back(std::string(str.substr(start)));
    return ret;
}

std::string Replace(std::string_view str, std::string_view from, std::string_view to)
{
    std::string ret(str);
    size_t start = 0;
    while ((start = ret.find(from, start)) != std::string::npos)
    {
        ret.replace(start, from.length(), to);
        start += to.length();
    }
    return ret;
}

std::string Replace(std::string_view str, char from, char to)
{
    std::string ret(str);
    for (char& c : ret)
    {
        if (c == from)
        {
            c = to;
        }
    }
    return ret;
}

void ReplaceIn(std::string& str, std::string_view from, std::string_view to)
{
    size_t start = 0;
    while ((start = str.find(from, start)) != std::string::npos)
    {
        str.replace(start, from.length(), to);
        start += to.length();
    }
}

void CommentFunction(std::string& sourceCode, std::string_view signature)
{
    size_t startPos = sourceCode.find(signature);
    if (startPos == std::string::npos)
    {
        PT_ASSERT_MSG(false, "Function not found: ", signature);
        return;
    }
    size_t braceLevel = 0;

    sourceCode.insert(startPos, "/*");

    for (size_t i = startPos + 2 + signature.length(); i < sourceCode.size(); ++i)
    {
        if (sourceCode[i] == '{')
        {
            ++braceLevel;
        }
        else if (sourceCode[i] == '}')
        {
            --braceLevel;
            if (braceLevel == 0)
            {
                sourceCode.insert(i + 1, "*/");
                break;
            }
        }
    }
}

void RemoveFunction(std::string& sourceCode, std::string_view signature)
{
    size_t startPos = sourceCode.find(signature);
    if (startPos == std::string::npos)
    {
        PT_ASSERT_MSG(false, "Function not found: ", signature);
        return;
    }
    size_t braceLevel = 0;

    for (size_t i = startPos + signature.length(); i < sourceCode.size(); ++i)
    {
        if (sourceCode[i] == '{')
        {
            ++braceLevel;
        }
        else if (sourceCode[i] == '}')
        {
            --braceLevel;
            if (braceLevel == 0)
            {
                sourceCode.erase(startPos, i - startPos + 1);
                break;
            }
        }
    }
}

} // namespace Pt