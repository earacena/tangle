
#include <map>
#include <string>
#include <vector>

#include "../src/include/argument_parser.h"

int main(int argc, char **argv) {

  Command create_command = {
      "c",
      "create",
      "Creates a new entry.",
      "Creates and stores a new entry in the journal file.",
      1,
      [](const std::vector<std::string> &args) {
        std::cout << "Creating entry using args: \n";

        for (const auto &arg : args) {
          std::cout << arg << " ";
        }
        if (args.size() == 0) {
          std::cout << "no arguments\n";
        }

        std::cout << '\n';
      }};

  std::map<std::string, Command> command_table = {{"c", create_command}};

  auto argparse = ArgumentParser(command_table);
}
