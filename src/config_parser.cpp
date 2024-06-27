#include "include/config_parser.h"

#include "include/util.h"

auto ConfigParser::Parse(const std::vector<ConfigValue>& values)
    -> std::unordered_map<std::string, std::string> {
  std::unordered_map<std::string, std::string> config;

  std::string env_var_value = "";
  for (const auto& value : values) {
    // Retrieve env var value and set to given config field name, if env var is
    // not set, use fallback value
    env_var_value = GetEnvVarString(value.env_var);

    config[value.config_field_name] =
        !env_var_value.empty() ? env_var_value : value.fallback_val;
  }

  return config;
}
