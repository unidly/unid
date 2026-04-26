/**
 * @file
 * @brief Returns a filepath based on XDG logic
 * @copyright Copyright 2036, Unidly LLC. All rights reserved.
 */

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

/**
 * @brief Returns the path where unid can access the unid.toml file
 *
 * The location of the unid.toml configuration can be located in one of two
 * places: ~/.config/unid.toml or /etc/unid/unid.toml. If the file is
 * found in the home directory, that file us used as it has priority over the
 * system configuration file. If there is no ~/.config/Unid.toml or of $HOME is
 * not set, then /etc/unid/unid.toml is used.
 *
 * @note The use of the home directory based configuration file should only
 * be used during development. Outside of the development environment, the
 * /etc/unid/unid.toml file should be used and the unid.toml file should be
 * removed from ~/.config
 *
 * Algorithm:
 * If environment variable HOME is set
 *   search for unid.conf in $HOME/.config/unid.toml
 *   if found, return the filepath "~/.config/unid.toml"
 *   If not found, search for unid.conf in /etc/unid/unid.toml
 *     if found, return "/etc/unid/unid.toml"
 *     if not found, throw runtime exception "unid.toml. not found"
 * else
 *   search for unid.conf in /etc/unid/unid.toml
 *   if found, return "/etc/unid/unid.toml"
 *   else throw runtime exception "unid.toml not found"
 *
 * @throws runtime exception if configuration file not found.
 */
std::string get_unid_filepath() {
  std::string filepath;
  std::string home = std::getenv("HOME");
  if (!home.empty()) {
    filepath = home + "/.config/unid.toml";
    fs::path p(filepath);
    if (fs::exists(p)) {
      return filepath;
    }
  }

  filepath = "/etc/unid/unid.toml";
  fs::path p(filepath);
  if (fs::exists(p)) {
    return filepath;
  }

  throw std::runtime_error("Configuration file, unid.toml, not found");
}

int main() {
  std::string filepath;
  try {
    filepath = get_unid_filepath();
  } catch (const std::exception& e) {
    std::cout << "Error: " << e.what() << std::endl;
  }
  std::cout << filepath << std::endl;
  return 0;
}
