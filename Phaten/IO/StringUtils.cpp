#include "StringUtils.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>

#include "IO/Assert.hpp"

namespace Pt {

/// Process ===================================================================
/// ===========================================================================

std::string ReadFile(std::string_view path)
{
    std::ifstream file(path.data(), std::ios::in | std::ios::ate);
    if (!file.is_open())
    {
        return "";
    }
    std::string content;
    size_t size = file.tellg();
    content.resize(size);
    file.seekg(0, std::ios::beg);
    file.read(&content[0], size);
    file.close();
    return content;
}

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
    if (str.empty())
    {
        return ret;
    }
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

size_t IndexOfList(std::string_view str, const std::string* list, size_t defaultIdx)
{
    size_t idx = 0;
    for (const std::string* ptr = list; ptr->size() > 0; ++ptr)
    {
        if (ptr->compare(str) == 0)
        {
            return idx;
        }
        ++idx;
    }
    return defaultIdx;
}

int ParseInt(std::string_view str)
{
    return std::stoi(std::string(str));
}

bool StartWith(std::string_view str, std::string_view substr)
{
    return str.find(substr) == 0;
}

void TrimSpace(std::string& str)
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos)
    {
        str.clear();
        return;
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    str = str.substr(start, end - start + 1);
}
/// Output ====================================================================
/// ===========================================================================

#ifdef PT_SHADER_DEBUG
static int FindErrorLineNumber(std::string_view str)
{
    std::string errorToken = "ERROR:";
    size_t errorTokenPos = str.find(errorToken);
    if (errorTokenPos == std::string::npos)
    {
        return -1;
    }

    size_t lineNumStart = str.find_first_of(":", errorTokenPos + errorToken.size());
    size_t lineNumEnd = str.find_first_of(":", lineNumStart + 1);

    return ParseInt(str.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1));
}

void PrintShaderByLine(std::string_view str, std::string_view error)
{
    int errorLine = FindErrorLineNumber(error);

    std::istringstream stream(str.data());
    std::string line;
    int lineNum = 1;
    while (std::getline(stream, line))
    {
        if (lineNum == errorLine && errorLine > 0)
        {
            std::cout << "\x1b[31m" << std::setw(3) << lineNum << ": " << line << "\x1b[0m" << std::endl;
        }
        else
        {
            std::cout << std::setw(3) << lineNum << ": " << line << std::endl;
        }
        ++lineNum;
    }
}
#endif

} // namespace Pt