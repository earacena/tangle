#pragma once

#include <chrono>
#include <ctime>
#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sodium.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <time.h>
#include <vector>

typedef std::string JournalId;

const uint16_t JOURNAL_ENTRY_SIZE =
    533; // 4 bytes for id, 4 bytes for size, 25 bytes for date string, 500
         // bytes for padded content
const uint16_t CONTENT_MAX_SIZE = 500; // 500 bytes

class JournalEntry {
public:
  JournalEntry() {
    // Timestamp new entry
    auto now = std::chrono::system_clock::now();
    timestamp = std::chrono::system_clock::to_time_t(now);
  };

  JournalEntry(int id, std::time_t t, std::string &content)
      : timestamp(std::move(t)), content(std::move(content)){};

  JournalEntry(int id, std::string &content)
      : id(id), content(std::move(content)) {
    // Timestamp new entry
    auto now = std::chrono::system_clock::now();
    timestamp = std::chrono::system_clock::to_time_t(now);
  };

  auto Store(std::fstream &fs) -> void;
  auto Load(std::fstream &fs) -> void;

  int id;                // 4 bytes
  std::time_t timestamp; // 19 bytes, uses ISO 8601 date string
  std::string content;   // 500 bytes
};

struct JournalState {
  std::vector<JournalEntry> entries;
};

class DiskManager {
public:
  DiskManager(const std::string &filename);
  ~DiskManager();
  auto WriteEntry(JournalEntry &entry) -> void;
  auto ReadEntries() -> std::vector<JournalEntry>;

private:
  // Journal file
  std::string journal_name_;
  std::fstream journal_io_;
};

class Journal {
public:
  auto CreateEntry(std::string &content) -> bool;

  std::vector<JournalEntry> entries;

private:
  DiskManager dm_;
  int num_of_entries_ = 0;
};
