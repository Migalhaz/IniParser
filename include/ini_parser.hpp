/// @file ini_parser.hpp
/// @brief Header file for INI file parser class
/// @author Miguel Guion
/// @date 2025-09-28
/// @version 1.00
#pragma once
#ifndef INI_PARSER_HPP
#define INI_PARSER_HPP

#include <string>
#include <map>

#include "ini_exceptions.hpp"
#include "ini_config.hpp"

namespace ini // namespace to avoid name clashes
{

/// @brief Class for parsing INI files
class IniParser 
{
public:
    /// @brief Default constructor
    IniParser()  = default;

    /// @brief Default destructor
    ~IniParser() = default;

    /// @brief Create IniParser instance from a file
    /// @param filename Path to the INI file
    /// @param config Configuration for handling duplicates (uses default if not provided)
    /// @return IniParser instance with loaded data
    static IniParser from_file(const std::string& filename, const IniParserConfig& config = IniParserConfig());

    /// @brief Load INI file
    /// @param filename Path to the INI file
    /// @param config Configuration for handling duplicates (uses default if not provided)
    void load(const std::string& filename, const IniParserConfig& config = IniParserConfig());

    /// @brief Save INI data to a file
    /// @param filename Path to save the INI file
    /// @return True if saved successfully, false otherwise
    void save(const std::string& filename) const;
    
    /// @brief Get value by section and key with an optional default value
    /// @param section Section name
    /// @param key Key name
    /// @param default_value Default value if key or section not found, defaults to empty string 
    /// @return Value associated with the key in the section, or default value if not found
    std::string get(const std::string& section, const std::string& key, const std::string& defaultValue = "") const;

    /// @brief Convert INI data to string format
    /// @return String representation of the INI data
    std::string to_string() const;
    
    private:
    // Nested map to hold sections and their key-value pairs
    std::map<std::string, std::map<std::string, std::string>> _data;
    
    /// @brief Trim whitespace from both ends of a string
    /// @param str Input string to trim
    /// @return Trimmed string
    std::string trim(const std::string& str); // Trim whitespace from both ends of a string
    
    /// @brief Turns all characters to lower case
    /// @param str Input string to lower 
    /// @return lowered string
    std::string to_lower(const std::string& str); // Lower all chars of a string
};

} // namespace ini
#endif // INI_PARSER_HPP