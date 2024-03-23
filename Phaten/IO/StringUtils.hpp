#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace Pt {

std::string FormatString(const char* format, ...);

template <typename... Args>
std::string StringConcatenate(char connector, const std::string& first, Args... args)
{
    std::string ret;
    ret.append(first);
    ((ret.push_back(connector), ret.append(args)), ...);
    return ret;
}

template <typename... Args>
std::string StringViewConcatenate(char connector, std::string_view first, Args... args)
{
    std::string ret;
    ret.append(first);
    ((ret.push_back(connector), ret.append(args)), ...);
    return ret;
}

/// Split string by delimiter.
std::vector<std::string> Split(std::string_view str, char delimiter = ' ');
/// Replace a string in a string and return the new string.
std::string Replace(std::string_view str, std::string_view from, std::string_view to);
/// Replace a character in a string and return the new string.
std::string Replace(std::string_view str, char from, char to);

/// Replace a string in a string
void ReplaceIn(std::string& str, std::string_view from, std::string_view to);


/// Comment out a function in shader source code.
void CommentFunction(std::string& sourceCode, std::string_view signature);
/// Remove a function in shader source code.
void RemoveFunction(std::string& sourceCode, std::string_view signature);

} // namespace Pt