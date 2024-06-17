
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../src/include/argument_parser.h"
#include "../src/include/journal.h"

int main(int argc, char **argv) {
  // Create journal and components
  DiskManager dm{"main.journal"};
  Journal j{dm};

  auto create_command = std::make_shared<Command>(
      "c", "create", "Creates a new entry.",
      "Creates and stores a new entry in the journal file.", 1,
      [](Journal &journal, const std::vector<std::string> &args) {
        std::cout << "Creating entry...\n";
        std::string content = args.at(0);
        const int id = journal.CreateEntry(content);
        std::cout << std::format("Entry created (id: {})\n", id);

        // std::cout << "Verifying...\n";
        // auto entry = journal.GetEntry(id);
        // if (entry.content != content) {
        //   std::cout
        //       << "Verification failed, entry content is not consistent.\n";
        // }
      });

  auto list_command = std::make_shared<Command>(
      "l", "list", "Lists all entries.",
      "Lists all the entries stored in journal file.", 0,
      [](Journal &journal, const std::vector<std::string> &args) {
        journal.ListAllEntries();
      });

  std::map<std::string, std::shared_ptr<Command>> command_table = {
      {"c", create_command},
      {"create", create_command},
      {"l", list_command},
      {"list", list_command}};

  auto argparse = ArgumentParser(command_table);

  auto result = argparse.Parse(argc, argv);

  if (!result.has_value()) {
    argparse.PrintUsage();
  } else {
    auto pairs = result.value();
    for (const auto &pair : pairs) {
      pair.command->apply_fn(j, pair.args);
    }
  }
}
