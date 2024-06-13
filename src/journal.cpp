#include "include/journal.h"

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
    entries.push_back(entry);
    dm_.WriteEntry(entry);
    ++num_of_entries_;
  }

  return true;
}

DiskManager::DiskManager(const std::string &filename) {
  journal_io_.open(filename, std::ios::binary | std::ios::in | std::ios::out |
                                 std::ios::app);

  if (!journal_io_.is_open()) {
    throw std::exception();
  }
}

DiskManager::~DiskManager() { journal_io_.close(); }

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
