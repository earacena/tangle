#include "../src/include/argument_parser.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Parses arguments", "[parsing]") {
  SECTION("Correctly parses arguments") {
    int argc = 3;

    const char *argv[argc + 1];
    argv[0] = "tangle";
    argv[1] = "-c";
    argv[2] = "This is an entry";
    argv[3] = NULL;

    std::map<std::string, Command> command_table;

    Command create_command = {
        "c",
        "create",
        "Creates an entry.",
        "Creates an entry then stores it in the journal file",
        1,
        [](std::vector<std::string> args) {
          // Create JournalEntry
          // Store JournalEntry
        }};

    auto argparse = ArgumentParser({{"c", create_command}});

    auto result = argparse.Parse(argc, const_cast<char **>(argv));

    REQUIRE(result.has_value() == true);

    auto pairs = result.value();

    REQUIRE(pairs.at(0).command.short_name == "c");
    REQUIRE(pairs.at(0).args.at(0) == "This is an entry");
  }

  SECTION("Returns null optional when parsing invalid arguments") {

    int argc = 2;

    const char *argv[argc + 1];
    argv[0] = "tangle";
    argv[1] = "-c";
    argv[2] = NULL;

    std::map<std::string, Command> command_table;

    Command create_command = {
        "c",
        "create",
        "Creates an entry.",
        "Creates an entry then stores it in the journal file",
        1,
        [](std::vector<std::string> args) {
          // Create JournalEntry
          // Store JournalEntry
        }};

    auto argparse = ArgumentParser({{"c", create_command}});

    auto result = argparse.Parse(argc, const_cast<char **>(argv));

    REQUIRE(result.has_value() == false);
  }
}
