#include "ini_parser.hpp"

#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>

ini::IniParser ini::IniParser::from_file(const std::string& filename, const IniParserConfig& config)
{
    IniParser parser; // Create an instance of IniParser
    parser.load(filename, config); // Load the INI file
    return parser; // Return the populated instance
}

void ini::IniParser::load(const std::string& filename, const IniParserConfig& config) 
{
    if (filename.empty()) // Check for empty filename
    {
        throw ini::FileError("Filename is empty"); // Throw exception
    }

    std::ifstream file(filename); // Open the file
    if (!file.is_open()) // Check if file opened successfully
    {
        throw ini::FileError("Could not open file: " + filename); // Throw exception
    }

    std::string line; // To hold each line
    std::string currentSection; // To track the current section

    int currentLineNumber = 1; // To track line numbers for error reporting
    while (std::getline(file, line)) // Read file line by line
    {
        line = trim(line); // Trim whitespace
        if (line.empty() || line[0] == ';' || line[0] == '#') // Skip comments and empty lines
        {
            ++currentLineNumber;
            continue; // Skip comments and empty lines
        }

        // Section header
        if (line.front() == '[' && line.back() == ']')
        {
            currentSection = line.substr(1, line.size() - 2); // Extract section name

            if ((config._loadFlags & LoadFlags::CaseSensitive) == 0)
            {
                to_lower(currentSection);
            }

            if (_data.find(currentSection) != _data.end())
            {
                if (config._sectionPolicy == DuplicateSectionPolicy::Ignore)
                {
                    ++currentLineNumber; // Increment line number
                    continue; // Ignore duplicate section
                }
                else if (config._sectionPolicy == DuplicateSectionPolicy::Overwrite)
                {
                    _data[currentSection].clear(); // Clear existing section
                }
                else if (config._sectionPolicy == DuplicateSectionPolicy::Merge)
                {
                    // Merge policy: do nothing, just continue to add to existing section
                }
                else // Throw_Error
                {
                    throw ini::ParseError("Duplicate section: " + currentSection, currentLineNumber); // Throw exception
                }
            }
            else
            {
                _data[currentSection] = {}; // Initialize section
            }
            ++currentLineNumber; // Increment line number
            continue; // Move to next line
        }

        // Key-value pair
        size_t delimiterPos = line.find('='); // Find the '=' delimiter
        if (delimiterPos == std::string::npos) // No delimiter found
        {
            throw ini::ParseError("Invalid line (no '=' found): " + line, currentLineNumber); // Throw exception
        }

        std::string key = line.substr(0, delimiterPos); // Extract key
        std::string value = line.substr(delimiterPos + 1); // Extract value

        // Trim whitespace from key and value

        if (config._whitespacePolicy == WhitespacePolicy::Trim)
        {
            key = trim(key);
        }
        if (key.empty()) // Check for empty key
        {
            if ((config._loadFlags & LoadFlags::AllowNoKey) == 0)
            {
                throw ini::ParseError("Empty key in line: " + line, currentLineNumber); // Throw exception
            }
        }

        if (config._whitespacePolicy == WhitespacePolicy::Trim)
        {
            value = trim(value);
        }
        if (value.empty()) // Check for empty value
        {
            if ((config._loadFlags & LoadFlags::AllowNoValue) == 0)
            {
                throw ini::ParseError("Empty value for key: " + key + " in line: " + line, currentLineNumber); // Throw exception
            }
        }
        if ((config._loadFlags & LoadFlags::CaseSensitive) == 0)
        {
            to_lower(value);
        }

        // Store key-value pair in the current section
        if (currentSection.empty() && (config._loadFlags & LoadFlags::AllowNoSection) == 0)
        {
            throw ini::ParseError("Key-value pair found outside of a section: " + line, currentLineNumber); // Throw exception
        }
        
        if (_data[currentSection].find(key) == _data[currentSection].end())
        {
            _data[currentSection][key] = value; // Store key-value pair
            ++currentLineNumber; // Increment line number
            continue;
        }

        switch (config._keyPolicy)
        {
            case DuplicateKeyPolicy::Throw_Error:
                throw ini::ParseError("Duplicated key: " + key, currentLineNumber);
            case DuplicateKeyPolicy::Merge:
                _data[currentSection][key] += value; // Merge key-value pair
                break;
            case DuplicateKeyPolicy::Overwrite:
                _data[currentSection][key] = value; // Overwrite key-value pair
                break;
            case DuplicateKeyPolicy::Ignore:
                // Ignore Policy: do nothing, just continue to ignore the line
                break;
        }
    
        ++currentLineNumber;
    }

    file.close(); // Close the file
}

void ini::IniParser::save(const std::string& filename) const
{
    if (filename.empty()) // Check for empty filename
    {
        throw ini::FileError("Filename is empty"); // Throw exception
    }

    std::ofstream file(filename); // Open the file for writing
    if (!file.is_open()) // Check if file opened successfully
    {
        throw ini::FileError("Could not open file for writing: " + filename); // Throw exception
    }
    
    file << to_string(); // Write the INI data to the file
    file.close(); // Close the file
}

std::string ini::IniParser::get(const std::string& section, const std::string& key, const std::string& defaultValue) const
{
    if (section.empty() || key.empty()) // Check for empty section or key
    {
        return defaultValue; // Return default if invalid input
    };

    //type auto = std::map<std::string, std::map<std::string, std::string>>::const_iterator
    const auto& sectionIt = _data.find(section); // Find the section

    if (sectionIt == _data.end()) // Section not found
    {
        return defaultValue; // Return default if section not found
    }

    //type auto = std::map<std::string, std::string> 
    const auto& sectionMap = sectionIt->second; // Find the key in the section
    
    //type auto = std::map<std::string, std::string>::const_iterator
    const auto& keyIt = sectionMap.find(key); // Find the key in the section

    if (keyIt == sectionMap.end()) // Key found
    {
        return defaultValue; // Return default if key not found
    }

    return keyIt->second; // Return the found value by key
}

std::string ini::IniParser::trim(const std::string& str) 
{
    const std::string whitespace = " \t\n\r\f\v"; // Define whitespace characters
    size_t start = str.find_first_not_of(whitespace); // Find first non-whitespace character
    size_t end = str.find_last_not_of(whitespace); // Find last non-whitespace character

    if (start == std::string::npos || end == std::string::npos) // String is all whitespace
    {
        return ""; // Return empty string
    }

    return str.substr(start, end - start + 1); // Return trimmed string
}

std::string ini::IniParser::to_lower(const std::string& str)
{
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), [](u_char c)
    {
        return std::tolower(c);
    });
    return result;
}

std::string ini::IniParser::to_string() const
{
    std::ostringstream oss; // Use ostringstream for efficient string construction
    for (const auto& [section, keyValues] : _data) // Structured binding (C++17)
    {
        oss << '[' << section << "]\n"; // Section header
        for (const auto& [key, value] : keyValues) // Structured binding (C++17)
        {
            oss << key << " = " << value << '\n'; // Key-value pair
        }
    }
    return oss.str(); // Return the constructed string
}