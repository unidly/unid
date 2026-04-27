/**
 * @file
 * @brief Contains common string constants
 * @copyright Copyright 2026, Unidly LLC. All rights reserved.
 */

/**
 * String Constants Rules
 * ----------------------
 * All string constants are store as inline constexpr string_views.
 * Organize string constant names in this file ALPHABETICALLY!
 * Name string constants in a way that groups related constants
 */

#ifndef STRING_CONSTANTS_HPP
#define STRING_CONSTANTS_HPP

// clang-format off
inline constexpr std::string_view config_home_filepath{"~/.config/unid.toml"};
inline constexpr std::string_view config_system_filepath{"/etc/unid/unid.toml"};
// clang-format on

#endif // STRING_CONSTANTS_HPP
