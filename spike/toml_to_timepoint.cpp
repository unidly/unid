// gcc14-g++ toml_to_timepoint.cpp -o bin/toml_to_timepoint
#include <chrono>
#include <iostream>
#include <toml.hpp>

int main() {
  // Example TOML data
  const auto data = toml::parse("start_time = 2026-04-03T12:30:10-07:00");

  // Get the datetime value with offset
  const auto& toml_dt = data["start_time"];
  std::cout << "Date/Time: " << toml_dt << std::endl;

  auto dt = data["start_time"].value<toml::date_time>();
  std::cout << "Year: " << dt->date.year << std::endl;
  std::cout << "Month: " << (int)dt->date.month << std::endl;
  std::cout << "Day: " << (int)dt->date.day << std::endl;

  std::cout << "Hour: " << (int)dt->time.hour << std::endl;
  std::cout << "Minute:: " << (int)dt->time.minute << std::endl;
  std::cout << "Second:: " << (int)dt->time.second << std::endl;

  if (dt->offset) {
    std::cout << "Offset (minutes): " << dt->offset->minutes << std::endl;
  }

  return 0;
}
