#include "include/journal.h"

auto Journal::CreateEntry(std::string &content) -> int {
  // Split the content into multiple entries if too big
  int id = num_of_entries_;  // id of entry or first chunk
  if (content.size() > CONTENT_MAX_SIZE) {
    std::vector<std::string> split_contents;
    for (int i = 0; i < content.size(); i += CONTENT_MAX_SIZE) {
      split_contents.push_back(content.substr(i, CONTENT_MAX_SIZE));
    }

    for (auto chunk : split_contents) {
      JournalEntry entry = {true, id, chunk};
      dm_.WriteEntry(entry);
      entries_.push_back(entry);
      ++num_of_entries_;
    }

    return id;
  }

  JournalEntry entry = {false, id, content};
  dm_.WriteEntry(entry);
  entries_.push_back(entry);
  ++num_of_entries_;

  return id;
}

auto Journal::LoadAllEntries() -> void {
  entries_ = dm_.ReadEntries();
  num_of_entries_ = entries_.size();

  std::cout << std::format("Loaded {} entries.\n", num_of_entries_);
}

auto Journal::GetAllEntries() -> std::vector<JournalEntry> { return entries_; }

auto Journal::ListAllEntries() -> void {
  // Used for converting time to string
  std::stringstream ss;
  for (const auto &entry : entries_) {
    std::cout << std::format("\nid: {}\ntimestamp: {}\n-------\n{}\n-------\n",
                             entry.id, SecondsToDateString(entry.timestamp),
                             entry.content);
  }
}

auto Journal::GetEntry(int id) -> JournalEntry { return dm_.ReadEntry(id); }
