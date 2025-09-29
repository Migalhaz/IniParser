#pragma once
#ifndef INI_CONFIG_HPP
#define INI_CONFIG_HPP

namespace ini // namespace to avoid name clashes
{

/// @brief Policy for handling duplicate keys in the same section
/// @details
/// - Throw_Error: Throws an error on duplicate key
/// - Overwrite: Overwrites the existing key with the new value
/// - Merge: Merges values (implementation-defined)
/// - Ignore: Ignores the new key-value pair
/// @note Default behavior is Throw_Error
enum class DuplicateKeyPolicy
{
    Throw_Error, // Throw error on duplicate key
    Overwrite,   // Overwrite existing key
    Merge,       // Merge values
    Ignore,      // Ignore new key
};

/// @brief Policy for handling duplicate sections
/// @details
/// - Throw_Error: Throws an error on duplicate section
/// - Overwrite: Overwrites the existing section with the new one
/// - Merge: Merges sections (implementation-defined)
/// - Ignore: Ignores the new section
/// @note Default behavior is Throw_Error
enum class DuplicateSectionPolicy
{
    Throw_Error, // Throw error on duplicate section
    Overwrite,   // Overwrite existing section
    Merge,       // Merge sections
    Ignore,      // Ignore new section
};

/// @brief Policy for handling whitespace in keys and values
/// @details
/// - Trim: Trims whitespace from keys and values
/// - Preserve: Preserves whitespace as is
/// @note Default behavior is Trim
enum class WhitespacePolicy
{
    Trim,        // Trim whitespace from keys and values
    Preserve,    // Preserve whitespace as is
};

/// @brief Flags for loading behavior
/// @details
/// - None: No special flags
/// - CaseSensitive: Treat section and key names as case-sensitive
/// - AllowEmptyValues: Allow keys with empty values
/// - AllowNoSection: Allow key-value pairs outside of any section
/// @note Flags can be combined using bitwise OR
/// @note Default behavior is CaseSensitive
enum class LoadFlags
{
    None            = 0, // No special flags
    CaseSensitive   = 1 << 0, // Treat section and key names as case-sensitive
    AllowNoSection  = 1 << 1, // Allow key-value pairs outside of any section
    AllowNoKey      = 1 << 2, // Allow empty keys
    AllowNoValue    = 1 << 3, // Allow empty values
};

inline LoadFlags operator|(LoadFlags a, LoadFlags b)
{
    return static_cast<LoadFlags>(static_cast<int>(a) | static_cast<int>(b));
}

inline int operator&(LoadFlags a, LoadFlags b)
{
    return static_cast<int>(a) & static_cast<int>(b);
}

/// @brief Configuration struct for IniParser
/// @details Holds policies for handling duplicates
struct IniParserConfig
{
    /// @brief Policy for handling duplicate sections
    DuplicateSectionPolicy _sectionPolicy = DuplicateSectionPolicy::Throw_Error; // Policy for duplicate sections
    
    /// @brief Policy for handling duplicate keys
    DuplicateKeyPolicy _keyPolicy = DuplicateKeyPolicy::Throw_Error; // Policy for duplicate keys
    
    /// @brief Flags for loading behavior
    LoadFlags _loadFlags = LoadFlags::CaseSensitive; // Flags for loading behavior
    
    /// @brief Policy for handling whitespace in keys and values
    WhitespacePolicy _whitespacePolicy = WhitespacePolicy::Trim; // Policy for handling whitespace
};

} // namespace ini

#endif // INI_CONFIG_HPP
