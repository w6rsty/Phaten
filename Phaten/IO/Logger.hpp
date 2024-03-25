#pragma once

#include <string>
#include <string_view>
#include <cassert>
#include <iostream>
#include <functional>
#include <unordered_map>

#include "Core/Core.hpp"
#include "Object/Ptr.hpp"

namespace Pt
{

enum class Level
{
    None = -1,
    Debug = 0,
    Trace,
    Info,
    Warn,
    Error,
    Fatal,
};

static const std::string_view LevelToString(Level level)
{
    switch (level)
    {
        case Level::Trace:  return "[Trace]";
        case Level::Debug:  return "[Debug]";
        case Level::Info:   return "[Info]";
        case Level::Warn:   return "[Warn]";
        case Level::Error:  return "[Error]";
        case Level::Fatal:  return "[Fatal]";
        case Level::None:   return "[]";
    }
}

/// Manage loggering to a certain stream.
/// Notice that stream can not change once been set.
class Logger final : public RefCounted
{
public:
    Logger(std::ostream& stream, Level level = Level::None, std::function<void(void)> createFn = nullptr)
    : stream_(stream), level_(level)
    {
        if (createFn) createFn();
    }

    ~Logger()
    {
        if (destoryFn_) destoryFn_();
    }

    void SetLevel(Level level) { level_ = level; }
    Level GetLevel() const { return level_; }

    /// Set custom destory callback.
    void SetDestoryCallback(std::function<void()> fn) { destoryFn_ = fn; }

    template <typename... Args>
    void Debug(std::string_view fileName, std::string function, uint32_t line, Args&&... args)
    {
        LogImpl(Level::Debug, fileName, function, line, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Trace(std::string_view fileName, std::string function, uint32_t line, Args&&... args)
    {
        LogImpl(Level::Trace, fileName, function, line, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Info(std::string_view fileName, std::string function, uint32_t line, Args&&... args)
    {
        LogImpl(Level::Info, fileName, function, line, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Warn(std::string_view fileName, std::string function, uint32_t line, Args&&... args)
    {
        LogImpl(Level::Warn, fileName, function, line, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Error(std::string_view fileName, std::string function, uint32_t line, Args&&... args)
    {
        LogImpl(Level::Error, fileName, function, line, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void Fatal(std::string_view fileName, std::string function, uint32_t line, Args&&... args)
    {
        LogImpl(Level::Fatal, fileName, function, line, std::forward<Args>(args)...);
    }
private:
    template <typename... Args>
    void LogImpl(Level level, std::string_view fileName, std::string funcName, uint32_t line, Args&&... args)
    {
        // filter out low level logs
        if (level < level_) return;

        stream_ << LevelToString(level) << " " << fileName << ":" << line << ":" << funcName << "\n";
        stream_ << ">> "; 
        doLogImpl(std::forward<Args>(args)...);
        stream_ << "\n";
    }

    template <typename Param, typename... Params>
    void doLogImpl(Param&& param, Params&&... params)
    {
        stream_ << param;
        doLogImpl(std::forward<Params>(params)...);
    }

    template <typename Param>
    void doLogImpl(Param&& param)
    {
        stream_ << param;
    }
private:
    std::ostream& stream_;
    Level level_;

    std::function<void()> destoryFn_; 
};

class LoggerManager final : public RefCounted
{
public:
    LoggerManager() {
        defaultLogger_ = CreateShared<Logger>(std::cout);
    }

    static LoggerManager& Instance() {
        static SharedPtr<LoggerManager> instance;
        if (instance.Get() == nullptr) {
            instance = CreateShared<LoggerManager>();
        }
        return *instance;
    }

    Logger& GetDefaultLogger() { return *defaultLogger_; }
    const Logger& GetDefault() const { return *defaultLogger_; }

    Logger& AddLogger(const std::string& name, std::ostream& stream)
    {
        if (loggers_.find(name) != loggers_.end()) {
            assert(false);
        }
        Logger* logger = new Logger(stream);
        loggers_[name] = SharedPtr<Logger>(logger);
        return *logger;
    }

    Logger& GetLogger(const std::string& name) { return *loggers_[name]; }
    const Logger& GetLogger(const std::string& name) const { return *loggers_.at(name); }
private:
    SharedPtr<Logger> defaultLogger_;
    std::unordered_map<std::string, SharedPtr<Logger>> loggers_;
};

#ifdef PT_ENABLE_LOGGING
    #define PT_LOG_DEBUG(...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Debug(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define PT_LOG_TRACE(...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Trace(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define PT_LOG_INFO(...)  ::Pt::LoggerManager::Instance().GetDefaultLogger().Info(__FILE__,  __FUNCTION__, __LINE__, __VA_ARGS__)
    #define PT_LOG_WARN(...)  ::Pt::LoggerManager::Instance().GetDefaultLogger().Warn(__FILE__,  __FUNCTION__, __LINE__, __VA_ARGS__)
    #define PT_LOG_ERROR(...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
    #define PT_LOG_FATAL(...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Fatal(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)

    #define PT_TAG_DEBUG(tag, ...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Debug(__FILE__, __FUNCTION__, __LINE__, "\x1b[32m[", tag "]\x1b[0m ", __VA_ARGS__)
    #define PT_TAG_TRACE(tag, ...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Trace(__FILE__, __FUNCTION__, __LINE__, "\x1b[35m[", tag "]\x1b[0m ", __VA_ARGS__)
    #define PT_TAG_INFO(tag, ...)  ::Pt::LoggerManager::Instance().GetDefaultLogger().Info(__FILE__,  __FUNCTION__, __LINE__, "\x1b[35m[", tag "]\x1b[0m ", __VA_ARGS__)
    #define PT_TAG_WARN(tag, ...)  ::Pt::LoggerManager::Instance().GetDefaultLogger().Warn(__FILE__,  __FUNCTION__, __LINE__, "\x1b[35m[", tag "]\x1b[0m ", __VA_ARGS__)
    #define PT_TAG_ERROR(tag, ...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Error(__FILE__, __FUNCTION__, __LINE__, "\x1b[35m[", tag "]\x1b[0m ", __VA_ARGS__)
    #define PT_TAG_FATAL(tag, ...) ::Pt::LoggerManager::Instance().GetDefaultLogger().Fatal(__FILE__, __FUNCTION__, __LINE__, "\x1b[35m[", tag "]\x1b[0m ", __VA_ARGS__)
#else
    #define PT_LOG_DEBUG(...)
    #define PT_LOG_TRACE(...)
    #define PT_LOG_INFO(...)
    #define PT_LOG_WARN(...)
    #define PT_LOG_ERROR(...)
    #define PT_LOG_FATAL(...)

    #define PT_TAG_DEBUG(tag, ...)
    #define PT_TAG_TRACE(tag, ...)
    #define PT_TAG_INFO(tag, ...)
    #define PT_TAG_WARN(tag, ...)
    #define PT_TAG_ERROR(tag, ...)
    #define PT_TAG_FATAL(tag, ...)
#endif

} // namespace Pt