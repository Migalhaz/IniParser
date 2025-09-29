#pragma once
#ifndef INI_EXCEPTIONS_HPP
#define INI_EXCEPTIONS_HPP

#include <stdexcept>

namespace ini // namespace to avoid name clashes
{

/// @brief Base exception class for INI parser errors
class IniException : public std::runtime_error 
{
public:
    /// @brief Constructor with error message
    /// @param message Error message
    explicit IniException(const std::string& message) 
        : std::runtime_error(message) {}
};

/// @brief Exception for file-related errors
class FileError : public IniException 
{
public:
    /// @brief Constructor with error message
    /// @param message Error message
    explicit FileError(const std::string& message)
        : IniException("File Error: " + message) {}
};

/// @brief Exception for parsing errors
class ParseError : public IniException 
{
public:
    /// @brief Constructor with error message
    /// @param message Error message
    /// @param lineNumber Line number where the error occurred (optional)
    explicit ParseError(const std::string& message, int lineNumber = -1)
        : IniException("Parse Error: " + message + " on line: " + std::to_string(lineNumber)), 
        _lineNumber(lineNumber) {}

    /// @brief Get the line number where the error occurred
    /// @return Line number, or -1 if not specified
    int get_line_number() const { return _lineNumber; } ///< Get the line number where the error occurred

private:
    int _lineNumber; ///< Line number where the error occurred
};

} // namespace ini

#endif // INI_EXCEPTIONS_HPP