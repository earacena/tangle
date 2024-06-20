#include "../src/include/disk_manager.h"

#include <ctime>
#include <filesystem>
#include <iostream>
#include <random>

#include "catch2/catch_test_macros.hpp"

auto Prepare() -> void {
  // Remove all test related files
  std::filesystem::remove("test.journal")
      ? std::cout << "journal file cleaned\n"
      : std::cout << "journal file not found\n";
}

auto GenerateEntry(int id, int random_content_size) -> JournalEntry {
  // Generates a variable length entry

  std::string char_set =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0"
      "123456789!@#$%^&*(),./;'[]-=|{}";

  // prepare random engine for picking random chars from character set
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distrib(0, char_set.size() - 1);

  std::string content = "";
  content.resize(random_content_size);

  for (size_t i = 0; i < content.size(); i++) {
    content.at(i) = char_set.at(distrib(gen));
  }

  return {id, content};
}

TEST_CASE("Storing and loading entries", "[disk]") {
  SECTION("Storing and loading a single entry") {
    Prepare();

    // Create journal and entry using content
    std::string content = "This is an entry";
    JournalEntry entry = {0, content};
    auto expected_timestamp = entry.timestamp;
    DiskManager dm = {"test.journal"};

    // Store entry
    dm.WriteEntry(entry);

    // Load all entries
    auto loaded_entries = dm.ReadEntries();

    // Check if entry is the same
    REQUIRE(loaded_entries.size() == 1);
    REQUIRE(loaded_entries.at(0).id == 0);
    REQUIRE(std::difftime(loaded_entries.at(0).timestamp, expected_timestamp) ==
            0);
    REQUIRE(loaded_entries.at(0).content == "This is an entry");

    // Load only the target entry
    auto loaded_entry = dm.ReadEntry(0);
    REQUIRE(loaded_entry.id == 0);
    REQUIRE(std::difftime(loaded_entry.timestamp, expected_timestamp) == 0);
    REQUIRE(loaded_entry.content == "This is an entry");
  }

  SECTION("Storing and loading variable length entries within max size") {
    Prepare();

    DiskManager dm = {"test.journal"};

    // Create a hundred variable length journal entries
    // prepare random number engine for generating random lengths for entry
    // content
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, CONTENT_MAX_SIZE);

    std::vector<JournalEntry> entries;
    for (int i = 0; i < 100; i++) {
      entries.push_back(GenerateEntry(i, distrib(gen)));
    }

    // Store journal entries
    for (auto &entry : entries) {
      dm.WriteEntry(entry);
    }

    // Read journal entries
    auto read_entries = dm.ReadEntries();

    // Check if entries match
    REQUIRE(read_entries.size() == entries.size());

    for (size_t i = 0; i < entries.size(); i++) {
      REQUIRE(entries.at(i).id == read_entries.at(i).id);
      REQUIRE(std::difftime(entries.at(i).timestamp,
                            read_entries.at(i).timestamp) == 0);
      REQUIRE(entries.at(i).content == read_entries.at(i).content);
    }
  }

  // SECTION("Storing and loading a larger than max size entry") {
  //   Prepare();

  //   auto entry1 = GenerateEntry(0, CONTENT_MAX_SIZE);
  //   auto entry2 = GenerateEntry(1, CONTENT_MAX_SIZE);
  //   auto entry3 = GenerateEntry(2, CONTENT_MAX_SIZE);

  //   DiskManager dm = {"test.journal"};

  //   dm.WriteEntry(entry1);
  //   dm.WriteEntry(entry2);
  //   dm.WriteEntry(entry3);

  //   REQUIRE(std::filesystem::file_size("test.journal") ==
  //           JOURNAL_ENTRY_SIZE * 3);

  //   auto read_entries = dm.ReadEntries();

  //   REQUIRE(read_entries.size() == 3);
  //   REQUIRE(read_entries.at(0).is_chunk == true);
  //   REQUIRE(read_entries.at(0).content ==
  //           entry1.content.substr(0, CONTENT_MAX_SIZE));
  //   REQUIRE(read_entries.at(1).is_chunk == true);
  //   REQUIRE(read_entries.at(1).content ==
  //           entry2.content.substr(CONTENT_MAX_SIZE, CONTENT_MAX_SIZE));
  //   REQUIRE(read_entries.at(2).is_chunk == true);
  //   REQUIRE(read_entries.at(2).content ==
  //           entry3.content.substr(CONTENT_MAX_SIZE * 2, CONTENT_MAX_SIZE));

  //   auto read_entry = dm.ReadEntry(0);
  //   REQUIRE(read_entry.id == entry1.id);
  //   REQUIRE(read_entry.timestamp == entry1.timestamp);
  //   REQUIRE(read_entry.is_chunk == entry1.is_chunk);
  //   REQUIRE(read_entry.content ==
  //           entry1.content + entry2.content + entry3.content);
  // }
}
