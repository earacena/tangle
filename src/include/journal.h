#pragma once

#include <sodium.h>
#include <time.h>

#include <chrono>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

typedef std::string JournalId;

const uint16_t JOURNAL_ENTRY_SIZE =
    534;  // 1 byte for chunk indicator, 4 bytes for id, 4 bytes for size, 25
          // bytes for date string, 500 bytes for padded content
const uint16_t CONTENT_MAX_SIZE = 500;  // 500 bytes

class JournalEntry {
 public:
  JournalEntry() {
    // Timestamp new entry
    auto now = std::chrono::system_clock::now();
    timestamp = std::chrono::system_clock::to_time_t(now);
  };

  JournalEntry(int id, std::time_t t, std::string &content)
      : timestamp(std::move(t)), content(std::move(content)){};

  JournalEntry(bool is_chunk, int id, std::string &content)
      : is_chunk(is_chunk), id(id), content(std::move(content)) {}

  JournalEntry(int id, std::string &content)
      : id(id), content(std::move(content)) {
    // Timestamp new entry
    auto now = std::chrono::system_clock::now();
    timestamp = std::chrono::system_clock::to_time_t(now);
  };
  auto Store(std::fstream &fs) -> void;
  auto Load(std::fstream &fs) -> void;

  bool is_chunk;  // 1 bytes, indicates with following or preceeding entries
                  // are chunks of the same entry
  int id;         // 4 bytes
  std::time_t timestamp;  // 19 bytes, uses ISO 8601 date string
  std::string content;    // 500 bytes
};

struct JournalState {
  std::vector<JournalEntry> entries;
};

class DiskManager {
 public:
  DiskManager(const std::string &filename);
  auto WriteEntry(JournalEntry &entry) -> void;
  auto ReadEntries() -> std::vector<JournalEntry>;
  auto ReadEntry(int id) -> JournalEntry;

 private:
  // Journal file
  std::string journal_name_;
  std::fstream journal_io_;
};

class Journal {
 public:
  Journal(DiskManager &dm) : dm_(std::move(dm)) { LoadAllEntries(); };

  auto CreateEntry(std::string &content) -> bool;
  auto LoadAllEntries() -> void;
  auto GetEntry(int id) -> JournalEntry;
  auto GetAllEntries() -> std::vector<JournalEntry>;
  auto ListAllEntries() -> void;

 private:
  DiskManager dm_;
  int num_of_entries_ = 0;
  std::vector<JournalEntry> entries_;
};
