// gcc14-g++ toml_to_timepoint.cpp -o bin/toml_to_timepoint -std=c++20
#include <chrono>
#include <iostream>
#include <toml.hpp>

#include <iostream>

std::chrono::system_clock::time_point
to_chrono_timepoint(const toml::date_time& dt) {
  using namespace std::chrono;

  year_month_day d = year{dt.date.year} / dt.date.month / dt.date.day;
  sys_days sd{d};
  system_clock::time_point tp =
      sd + hours{dt.time.hour} + minutes{dt.time.minute} +
      seconds{dt.time.second} + nanoseconds{dt.time.nanosecond};
  if (dt.offset) {
    tp -= minutes{dt.offset->minutes};
  }

  return tp;
}

int main() {
  // Example TOML local date
  {
    using namespace std::chrono;
    const auto config = toml::parse("start_date = 2026-04-03");

    if (!config["start_date"]) {
      std::cout << "Key not found\n";
    } else {
      toml::date td = config["start_date"].as_date()->get();
      year_month_day ymd{year{(short)td.year}, month{td.month}, day{td.day}};
      std::cout << "Chrono ymd: " << ymd << std::endl << std::endl;
    }
  }

  // Example TOML local time
  {
    using namespace std::chrono;
    const auto config = toml::parse("start_time = 14:35:25");

    if (!config["start_time"]) {
      std::cout << "Key not found\n";
    } else {
      auto tt = config["start_time"].as_time()->get();
      hh_mm_ss tod{hours{tt.hour} + minutes{tt.minute} + seconds{tt.second}};
      std::cout << "Chrono tod: " << std::format("{:%T}", tod) << std::endl
                << std::endl;
    }
  }

  // Example TOML local datetime
  {
    using namespace std::chrono;
    const auto config = toml::parse("start_datetime = 2026-04-03T14:35:25");

    if (!config["start_datetime"]) {
      std::cout << "Key not found\n";
    } else {
      toml::date_time dtt = config["start_datetime"].as_date_time()->get();

      year_month_day d = year{dtt.date.year} / dtt.date.month / dtt.date.day;
      sys_days sd{d};
      system_clock::time_point tp =
          sd + hours{dtt.time.hour} + minutes{dtt.time.minute} +
          seconds{dtt.time.second} + nanoseconds{dtt.time.nanosecond};
      std::cout << "Chrono timepoint: " << tp << std::endl << std::endl;
    }
  }

  // Example TOML datetime with timezone offset
  {
    using namespace std::chrono;
    const auto config =
        toml::parse("start_offset_datetime = 2026-04-03T14:35:25-07:00");

    if (!config["start_offset_datetime"]) {
      std::cout << "Key not found\n";
    } else {
      toml::date_time dtt =
          config["start_offset_datetime"].as_date_time()->get();

      year_month_day d = year{dtt.date.year} / dtt.date.month / dtt.date.day;
      sys_days sd{d};
      system_clock::time_point tp =
          sd + hours{dtt.time.hour} + minutes{dtt.time.minute} +
          seconds{dtt.time.second} + nanoseconds{dtt.time.nanosecond};
      if (dtt.offset) {
        tp -= minutes{dtt.offset->minutes};
      }
      std::cout << "Chrono timepoint: " << tp << std::endl << std::endl;
    }

    return 0;
  }
}
