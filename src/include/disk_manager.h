#pragma once
#include <cstring>
#include <filesystem>
#include <string>

#include "journal_entry.h"

class DiskManager {
 public:
  DiskManager(const std::string &filename) : filename_(filename) {}
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
