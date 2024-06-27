#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include "util.h"

struct JournalConfig {
  std::string config_path;
  std::string data_path;
};

struct ConfigValue {
  std::string config_field_name;  // name of the field that the value of the
                                  // environment variable will associated with
  std::string env_var;            // environment variable to extract
  std::string fallback_val;  // fallback if environment varible not setnot set
};

class ConfigParser {
 public:
  auto Parse(const std::vector<ConfigValue>& values)
      -> std::unordered_map<std::string, std::string>;

 private:
  std::fstream config_io_;
};
