#include "../src/include/journal_entry.h"

#include <filesystem>
#include <random>

#include "catch2/catch_test_macros.hpp"

auto Prepare() -> void {
  std::filesystem::remove("test.journal")
      ? std::cout << "journal file deleted\n"
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

TEST_CASE("Storing journal entries", "[store]") {
  SECTION("Stores entry with content less than max size") {
    Prepare();
    auto entry = GenerateEntry(0, CONTENT_MAX_SIZE / 2);

    std::fstream journal_io;
    journal_io.open("test.journal", std::ios::binary | std::ios::in |
                                        std::ios::out | std::ios::app);

    if (!journal_io.good()) {
      std::cout << strerror(errno) << "\n";
    }

    if (!journal_io.is_open()) {
      throw std::runtime_error("Unable to open/create journal file");
    }

    entry.Store(journal_io);

    journal_io.close();

    REQUIRE(std::filesystem::exists("test.journal"));
    REQUIRE(std::filesystem::file_size("test.journal") == JOURNAL_ENTRY_SIZE);
  }

  SECTION("Stores entries with content size greater than max size") {
    Prepare();
    auto entry1 = GenerateEntry(0, CONTENT_MAX_SIZE);
    auto entry2 = GenerateEntry(0, CONTENT_MAX_SIZE);

    std::fstream journal_io{"test.journal", std::ios::binary | std::ios::in |
                                                std::ios::out | std::ios::app};

    if (!journal_io.is_open()) {
      throw std::runtime_error("Unable to open/create journal file");
    }

    entry1.Store(journal_io);
    entry2.Store(journal_io);

    journal_io.close();

    REQUIRE(std::filesystem::file_size("test.journal") ==
            JOURNAL_ENTRY_SIZE * 2);
  }
}

TEST_CASE("Loading journal entries", "[load]") {
  SECTION("Loads stored entry with content less than max size") {
    Prepare();
    auto entry = GenerateEntry(0, CONTENT_MAX_SIZE / 2);

    std::fstream journal_io{"test.journal", std::ios::binary | std::ios::in |
                                                std::ios::out | std::ios::app};

    if (!journal_io.is_open()) {
      throw std::runtime_error("Unable to open/create journal file");
    }

    entry.Store(journal_io);
    journal_io.close();

    REQUIRE(std::filesystem::file_size("test.journal") == JOURNAL_ENTRY_SIZE);

    JournalEntry loaded_entry = {};

    journal_io.open("test.journal", std::ios::binary | std::ios::in |
                                        std::ios::out | std::ios::app);
    loaded_entry.Load(journal_io);
    journal_io.close();

    REQUIRE(loaded_entry.id == entry.id);
    REQUIRE(std::difftime(loaded_entry.timestamp, entry.timestamp) == 0);
    REQUIRE(loaded_entry.content == entry.content);
  }

  SECTION(
      "Loads stored entry with content greater than max size as a single "
      "entry") {
    Prepare();
    auto entry1 = GenerateEntry(0, CONTENT_MAX_SIZE);
    entry1.is_chunk = true;
    auto entry2 = GenerateEntry(1, CONTENT_MAX_SIZE);
    entry2.is_chunk = true;
    auto entry3 = GenerateEntry(2, CONTENT_MAX_SIZE);
    entry3.is_chunk = true;

    std::fstream journal_io{"test.journal", std::ios::binary | std::ios::in |
                                                std::ios::out | std::ios::app};

    if (!journal_io.is_open()) {
      throw std::runtime_error("Unable to open/create journal file");
    }

    entry1.Store(journal_io);
    entry2.Store(journal_io);
    entry3.Store(journal_io);

    journal_io.close();

    REQUIRE(std::filesystem::file_size("test.journal") ==
            JOURNAL_ENTRY_SIZE * 3);

    JournalEntry chunk_entry1 = {};
    JournalEntry chunk_entry2 = {};
    JournalEntry chunk_entry3 = {};

    journal_io.open("test.journal", std::ios::binary | std::ios::in |
                                        std::ios::out | std::ios::app);

    journal_io.seekg(0);
    chunk_entry1.Load(journal_io);
    REQUIRE(chunk_entry1.is_chunk == entry1.is_chunk);
    REQUIRE(chunk_entry1.id == entry1.id);
    REQUIRE(std::difftime(chunk_entry1.timestamp, entry1.timestamp) == 0);
    REQUIRE(chunk_entry1.content == entry1.content);

    journal_io.seekg(JOURNAL_ENTRY_SIZE);
    chunk_entry2.Load(journal_io);
    REQUIRE(chunk_entry2.is_chunk == entry2.is_chunk);
    REQUIRE(chunk_entry2.id == entry2.id);
    REQUIRE(std::difftime(chunk_entry2.timestamp, entry2.timestamp) == 0);
    REQUIRE(chunk_entry2.content == entry2.content);

    journal_io.seekg(JOURNAL_ENTRY_SIZE * 2);
    chunk_entry3.Load(journal_io);
    REQUIRE(chunk_entry3.is_chunk == entry3.is_chunk);
    REQUIRE(chunk_entry3.id == entry3.id);
    REQUIRE(std::difftime(chunk_entry3.timestamp, entry3.timestamp) == 0);
    REQUIRE(chunk_entry3.content == entry3.content);

    journal_io.close();
  }
}
