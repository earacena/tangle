
#include <map>
#include <string>
#include <vector>

#include "../src/include/argument_parser.h"

int main(int argc, char **argv) {
  // Create journal and components
  DiskManager dm{"main.journal"};
  Journal j{dm};

  Command create_command = {
      "c",
      "create",
      "Creates a new entry.",
      "Creates and stores a new entry in the journal file.",
      1,
      [](Journal &journal, const std::vector<std::string> &args) {
        std::cout << "Creating entry using args: \n";

        for (const auto &arg : args) {
          std::cout << arg << " ";
        }
        if (args.size() == 0) {
          std::cout << "no arguments\n";
        }

        std::cout << '\n';
      }};

  std::map<std::string, Command> command_table = {{"c", create_command},
                                                  {"create", create_command}};

  auto argparse = ArgumentParser(command_table);

  auto result = argparse.Parse(argc, argv);

  if (!result.has_value()) {
    argparse.PrintUsage();
  } else {
    auto pairs = result.value();
    for (const auto &pair : pairs) {
      pair.command.apply_fn(j, pair.args);
    }
  }
}
