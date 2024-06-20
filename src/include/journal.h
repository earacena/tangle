#pragma once

#include <sodium.h>
#include <time.h>

#include <fstream>
#include <string>
#include <vector>

#include "disk_manager.h"
#include "journal_entry.h"

class Journal {
 public:
  Journal(DiskManager &dm) : dm_(std::move(dm)) { LoadAllEntries(); };

  auto CreateEntry(std::string &content) -> int;
  auto LoadAllEntries() -> void;
  auto GetEntry(int id) -> JournalEntry;
  auto GetAllEntries() -> std::vector<JournalEntry>;
  auto ListAllEntries() -> void;

 private:
  DiskManager dm_;
  int num_of_entries_ = 0;
  std::vector<JournalEntry> entries_;
};
