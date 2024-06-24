#pragma once

#include <chrono>
#include <ctime>
#include <stdexcept>
#include <string>

inline auto SecondsToDateString(std::time_t time) -> std::string {
  std::vector<char> date_arr = {};

  date_arr.resize(20);  // "YYYY-MM-DD HH:MM:SS" requires 19 bytes plus 1 byte
                        // for null terminator

  int res = std::strftime(date_arr.data(), date_arr.size(), "%F %T",
                          std::localtime(&time));
  if (res == 0) {
    throw std::runtime_error("error converting time to string (0 written)");
  }

  // Ignore null terminator in final string, it is handled automatically by
  // std::string
  return std::string(date_arr.begin(), date_arr.end() - 1);
}
