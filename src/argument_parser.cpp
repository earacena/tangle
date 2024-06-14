#include "include/argument_parser.h"

auto ArgumentParser::Parse(int argc, char *argv)
    -> std::optional<std::vector<CommandPair>> {

  std::vector<CommandPair> command_pairs;
  int i = 0;

  while (i < argc) {
    std::string arg = std::to_string(argv[i]);
    std::string name = "";
    size_t pos = 0;

    // dashes can be ignored
    while (pos < arg.size() && arg.at(pos) == '-') {
      ++pos;
    }

    if (pos >= arg.size()) {
      // All dashes is not a valid command
      return std::nullopt;
    }

    // Extract name
    name = arg.substr(pos, arg.size() - pos);

    // Check if command is in table
    if (command_table.find(name) == std::end(command_table)) {
      return std::nullopt;
    }

    auto command = command_table.at(name);

    // Check and track possible arguments to command
    std::vector<std::string> args;
    size_t count = 0;
    for (int j = i + 1; (j < argc) && (args.size() < command.num_of_args);
         j++) {
      args.push_back(std::to_string(argv[j]));
    }

    // Check if required number of args was satisfied
    if (command.num_of_args != args.size()) {
      return std::nullopt;
    }

    // store result
    command_pairs.push_back({
        std::move(command),
        std::move(args),
    });

    // Move onto the next possible command
    ++i;
  }

  return command_pairs;
}
