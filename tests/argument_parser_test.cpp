#include "../src/include/argument_parser.h"

#include <catch2/catch_test_macros.hpp>
#include <filesystem>

#include "../src/include/journal.h"

auto Prepare() -> void {
  // Remove all test related files
  std::filesystem::remove("test.journal")
      ? std::cout << "journal file cleaned\n"
      : std::cout << "journal file not found\n";
}

TEST_CASE("Parses arguments", "[parsing]") {
  SECTION("Correctly parses arguments") {
    int argc = 3;

    const char *argv[4];
    argv[0] = "tangle";
    argv[1] = "-c";
    argv[2] = "This is an entry";
    argv[3] = NULL;

    std::map<std::string, Command> command_table;

    auto create_command = std::make_shared<Command>(
        "c", "create", "Creates an entry.",
        "Creates an entry then stores it in the journal file", 1,
        [](Journal &journal, const std::vector<std::string> &args) {
          // Create JournalEntry
          // Store JournalEntry
        });

    auto argparse = ArgumentParser({{"c", create_command}});

    auto result = argparse.Parse(argc, const_cast<char **>(argv));

    REQUIRE(result.has_value() == true);

    auto pairs = result.value();

    REQUIRE(pairs.at(0).command->short_name == "c");
    REQUIRE(pairs.at(0).args.at(0) == "This is an entry");
  }

  SECTION("Returns null optional when parsing invalid arguments") {
    int argc = 2;

    const char *argv[3];
    argv[0] = "tangle";
    argv[1] = "-c";
    argv[2] = NULL;

    std::map<std::string, Command> command_table;

    auto create_command = std::make_shared<Command>(
        "c", "create", "Creates an entry.",
        "Creates an entry then stores it in the journal file.", 1,
        [](Journal &journal, const std::vector<std::string> &args) {
          // Create JournalEntry
          // Store JournalEntry
        });

    auto argparse = ArgumentParser({{"c", create_command}});

    auto result = argparse.Parse(argc, const_cast<char **>(argv));

    REQUIRE(result.has_value() == false);
  }
}
