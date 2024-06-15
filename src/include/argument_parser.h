#pragma once

#include "journal.h"
#include <functional>
#include <map>
#include <optional>
#include <vector>

struct Command {
  std::string short_name;        // used with single dash,  for example "-a"
  std::string long_name;         // used with double dash, for example "--apply"
  std::string short_description; // for usage
  std::string long_description;  // for usage
  int num_of_args;               // how many expected arguments
  std::function<void(std::vector<std::string>)>
      apply_fn; // function that is called to handle command
};

struct CommandPair {
  Command command;
  std::vector<std::string> args;
};

class ArgumentParser {
public:
  ArgumentParser(std::map<std::string, Command> command_table)
      : command_table(std::move(command_table)) {}

  auto Parse(int argc, char **argv) -> std::optional<std::vector<CommandPair>>;

private:
  std::map<std::string, Command> command_table;
};
