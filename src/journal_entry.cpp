#include "include/journal_entry.h"

#include <chrono>

auto JournalEntry::Store(std::fstream &fs) -> void {
  // Pad content if necessary to make stored entries uniform
  std::string padded_content = content;
  int content_size = content.size();

  while (padded_content.size() < CONTENT_MAX_SIZE) {
    padded_content.push_back(' ');
  }

  // Write journal entry using file stream
  fs.write(reinterpret_cast<char *>(&is_chunk), sizeof(bool));
  fs.write(reinterpret_cast<char *>(&id), sizeof(int));
  fs.write(reinterpret_cast<char *>(&content_size), sizeof(int));
  fs.write(reinterpret_cast<char *>(&timestamp),
           sizeof(std::chrono::system_clock::rep));
  fs.write(padded_content.c_str(), CONTENT_MAX_SIZE);
}

auto JournalEntry::Load(std::fstream &fs) -> void {
  int content_size = 0;
  std::vector<char> padded_content = {};
  padded_content.resize(CONTENT_MAX_SIZE + 1);

  // Read data from binary file
  fs.read(reinterpret_cast<char *>(&is_chunk), sizeof(bool));
  fs.read(reinterpret_cast<char *>(&id), sizeof(int));
  fs.read(reinterpret_cast<char *>(&content_size), sizeof(int));
  fs.read(reinterpret_cast<char *>(&timestamp),
          sizeof(std::chrono::system_clock::rep));
  fs.read(reinterpret_cast<std::fstream::char_type *>(&padded_content.front()),
          CONTENT_MAX_SIZE);

  // Process binary data
  // Process date string from file
  auto padded_content_str =
      std::string(padded_content.begin(), padded_content.end());
  content = padded_content_str.substr(0, content_size);
}
