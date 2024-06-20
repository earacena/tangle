#pragma once

#include <time.h>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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

  JournalEntry(int id, std::time_t t, std::string content)
      : timestamp(std::move(t)), content(std::move(content)){};

  JournalEntry(bool is_chunk, int id, std::string content)
      : is_chunk(is_chunk), id(id), content(std::move(content)) {
    auto now = std::chrono::system_clock::now();
    timestamp = std::chrono::system_clock::to_time_t(now);
  }

  JournalEntry(int id, std::string content)
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
