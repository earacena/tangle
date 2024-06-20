#include <filesystem>
#include <random>

#include "../src/include/argument_parser.h"
#include "../src/include/journal.h"
#include "catch2/catch_test_macros.hpp"

auto Prepare() -> void {
  // Remove all test related files
  std::filesystem::remove("test.journal")
      ? std::cout << "journal file cleaned\n"
      : std::cout << "journal file not found\n";
}

auto GenerateString(int length) -> std::string {
  std::string char_set =
      "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz0"
      "123456789!@#$%^&*(),./;'[]-=|{}";

  // prepare random engine for picking random chars from character set
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, char_set.size() - 1);

  std::string content = "";
  content.resize(length);

  for (size_t i = 0; i < content.size(); i++) {
    content.at(i) = char_set.at(distrib(gen));
  }

  return content;
}

TEST_CASE("create (-c) command", "[command]") {
  auto command = std::make_shared<Command>(
      "c", "create", "Creates an entry.",
      "Creates an entry and stores it as chunks if necessary.", 1,
      [](Journal& journal, std::vector<std::string> args) {
        // Create
        const int id = journal.CreateEntry(args.at(0));

        // Check
        auto entries = journal.GetAllEntries();
        REQUIRE(entries.at(0).id == id);

        auto entry = journal.GetEntry(id);
        REQUIRE(entry.id == id);
        REQUIRE(entry.content == args.at(0));
      });

  SECTION("creates an entry successfully") {
    Prepare();
    DiskManager dm{"test.journal"};
    Journal j{dm};
    std::vector<std::string> args{"This is an entry!"};
    command->apply_fn(j, args);
  }

  SECTION("creates an entry that is split into chunks successfully") {
    Prepare();
    DiskManager dm{"test.journal"};
    Journal j{dm};
    std::vector<std::string> args{GenerateString(1200)};
    command->apply_fn(j, args);
  }
}
