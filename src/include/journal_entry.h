#pragma once

#include <time.h>

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const uint16_t JOURNAL_ENTRY_SIZE = 517;  // 1 byte for chunk indicator
                                          // 4 bytes for id
                                          // 4 bytes for size
                                          // 8 bytes timestamp
                                          // 500 bytes for padded content

const uint16_t CONTENT_MAX_SIZE = 500;  // 500 bytes

class JournalEntry {
 public:
  JournalEntry() {
    // Timestamp new entry
    timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  };

  JournalEntry(int id, std::chrono::system_clock::rep t, std::string content)
      : timestamp(std::move(t)), content(std::move(content)){};

  JournalEntry(bool is_chunk, int id, std::string content)
      : is_chunk(is_chunk), id(id), content(std::move(content)) {
    timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  }

  JournalEntry(int id, std::string content)
      : id(id), content(std::move(content)) {
    // Timestamp new entry
    timestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::system_clock::now().time_since_epoch())
                    .count();
  };
  auto Store(std::fstream &fs) -> void;
  auto Load(std::fstream &fs) -> void;

  bool is_chunk;  // 1 bytes, indicates with following or preceeding entries
                  // are chunks of the same entry
  int id;         // 4 bytes
  std::chrono::system_clock::rep timestamp;  // 4 bytes, long
  std::string content;                       // 500 bytes
};
