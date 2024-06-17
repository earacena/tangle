#include "include/argument_parser.h"

auto ArgumentParser::Parse(int argc, char *argv[])
    -> std::optional<std::vector<CommandPair>> {
  if (argc == 1) {
    // No arguments besides executable name
    return std::nullopt;
  }

  std::vector<CommandPair> command_pairs;
  int i = 1;  // i = 0 is the name of the executable

  while (i < argc) {
    std::string arg = argv[i];
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
    for (int j = i + 1; (j < argc) && (args.size() < command->num_of_args);
         j++) {
      args.push_back(argv[j]);
      i++;
    }

    // Check if required number of args was satisfied
    if (command->num_of_args != args.size()) {
      return std::nullopt;
    }

    // store result
    command_pairs.push_back({
        command,
        std::move(args),
    });

    // Move onto the next possible command
    ++i;
  }

  return command_pairs;
}

auto ArgumentParser::PrintUsage() -> void {
  Command command;

  std::cout
      << "tangle - A encrypted commandline journaling tool written in C++20.\n";
  std::cout << "Usage:\n";

  for (auto it = std::begin(command_table); it != std::end(command_table);
       ++it) {
    auto command = it->second;

    std::cout << std::format("-{}, --{}\t\t{}\n", command->short_name,
                             command->long_name, command->long_description);
  }
}
