#pragma once
#include <cstring>
#include <filesystem>
#include <string>

#include "journal_entry.h"

class DiskManager {
 public:
  // journal header
  // 1 byte, version

  // Scheme for each entry
  // 1 byte, is_chunk
  // 4 bytes, id
  // 4 bytes, content size
  // 8 bytes, timestamp, (a long is guaranteed to be at least 4 bytes but can be
  // up to 8 bytes) at most 500 bytes, content/chunk
  DiskManager(const std::string &filename) : filename_(filename) {
    journal_io_.open(filename, std::ios::in | std::ios::out | std::ios::binary |
                                   std::ios::app);

    // Check if file and can be created/opened without issue
    if (!journal_io_.is_open()) {
      throw std::runtime_error(
          std::format("Error opening journal file: {}", strerror(errno)));
    }

    journal_io_.close();
  }
  auto WriteEntry(JournalEntry &entry) -> void;
  auto ReadEntries() -> std::vector<JournalEntry>;
  auto ReadEntry(int id) -> JournalEntry;

 private:
  auto OpenFileStream() -> void;
  auto CloseFileStream() -> void;

  // Journal file
  std::string filename_;
  std::fstream journal_io_;
};
