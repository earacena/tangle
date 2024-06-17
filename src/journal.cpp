#include "include/journal.h"

#include <sstream>
#include <stdexcept>

auto JournalEntry::Store(std::fstream &fs) -> void {
  // Pad content if necessary to make stored entries uniform
  std::string padded_content = content;
  int content_size = content.size();

  while (padded_content.size() < CONTENT_MAX_SIZE) {
    padded_content.push_back(' ');
  }

  char date_arr[25];
  // Convert timestamp into a portable string
  std::size_t res =
      std::strftime(date_arr, 25, "%c", std::localtime(&timestamp));
  if (res == 0) {
    throw std::runtime_error("unable to convert std::time_t to date string");
  }

  date_arr[24] = '\0';

  // Write journal entry using file stream
  fs.write(reinterpret_cast<char *>(&id), sizeof(int));
  fs.write(reinterpret_cast<char *>(&content_size), sizeof(int));
  fs.write(date_arr, 25);
  fs.write(padded_content.c_str(), CONTENT_MAX_SIZE);
}

auto JournalEntry::Load(std::fstream &fs) -> void {
  int content_size = 0;
  std::vector<char> padded_content = {};
  padded_content.resize(CONTENT_MAX_SIZE + 1);

  char date_arr[25];

  // Read data from binary file
  fs.read(reinterpret_cast<char *>(&id), sizeof(int));
  fs.read(reinterpret_cast<char *>(&content_size), sizeof(int));
  fs.read(date_arr, 25);
  fs.read(reinterpret_cast<std::fstream::char_type *>(&padded_content.front()),
          CONTENT_MAX_SIZE);

  // Process binary data
  // Process date string from file

  struct std::tm tm;
  strptime(date_arr, "%c", &tm);
  timestamp = mktime(&tm);

  auto padded_content_str =
      std::string(padded_content.begin(), padded_content.end());
  content = padded_content_str.substr(0, content_size);
}

auto Journal::CreateEntry(std::string &content) -> bool {
  // Split the content into multiple entries if too big
  std::vector<std::string> split_contents;
  if (content.size() > CONTENT_MAX_SIZE) {
    for (int i = 0; i < content.size(); i += CONTENT_MAX_SIZE) {
      split_contents.push_back(content.substr(i, CONTENT_MAX_SIZE));
    }
  }

  for (auto chunk : split_contents) {
    JournalEntry entry = {num_of_entries_, chunk};
    entries_.push_back(entry);
    dm_.WriteEntry(entry);
    ++num_of_entries_;
  }

  return true;
}

auto Journal::LoadAllEntries() -> void {
  entries_ = dm_.ReadEntries();
  num_of_entries_ = entries_.size();
}

auto Journal::GetAllEntries() -> std::vector<JournalEntry> { return entries_; }

auto Journal::ListAllEntries() -> void {
  // Used for converting time to string
  std::stringstream ss;
  for (const auto &entry : entries_) {
    ss = std::stringstream();  // Reset stringstream
    auto date_string = std::put_time(std::localtime(&entry.timestamp), "%c");
    ss << date_string;
    std::cout << std::format("\nid: {}\ntimestamp: {}\n-------\n{}\n-------\n",
                             entry.id, ss.str(), entry.content);
  }
}

auto Journal::GetEntry(int id) -> JournalEntry { return dm_.ReadEntry(id); }

DiskManager::DiskManager(const std::string &filename) {
  // Open journal file stream
  journal_io_.open(filename, std::ios::binary | std::ios::in | std::ios::out |
                                 std::ios::app);

  if (!journal_io_.is_open()) {
    throw std::exception();
  }
}

auto DiskManager::WriteEntry(JournalEntry &entry) -> void {
  entry.Store(journal_io_);
}

auto DiskManager::ReadEntries() -> std::vector<JournalEntry> {
  int pos = 0;
  int offset = JOURNAL_ENTRY_SIZE;
  std::vector<JournalEntry> entries = {};

  journal_io_.seekg(0);
  while (journal_io_.peek() != EOF) {
    JournalEntry entry = {};
    entry.Load(journal_io_);
    entries.push_back(std::move(entry));
    pos += offset;
    journal_io_.seekg(pos);
  }

  return entries;
}

auto DiskManager::ReadEntry(int id) -> JournalEntry {
  int pos = id * JOURNAL_ENTRY_SIZE;

  journal_io_.seekg(pos);

  if (journal_io_.eof()) {
    throw std::runtime_error(
        "entry not found at expected position of journal file (EOF)");
  }

  JournalEntry entry = {};
  entry.Load(journal_io_);

  return entry;
}
