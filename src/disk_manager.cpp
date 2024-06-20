#include "include/disk_manager.h"

#include "include/journal_entry.h"

auto DiskManager::WriteEntry(JournalEntry &entry) -> void {
  OpenFileStream();

  entry.Store(journal_io_);

  CloseFileStream();
}

auto DiskManager::ReadEntries() -> std::vector<JournalEntry> {
  OpenFileStream();

  int pos = 0;
  int offset = JOURNAL_ENTRY_SIZE;
  std::vector<JournalEntry> entries = {};

  journal_io_.seekg(0);
  while (journal_io_.peek() != EOF) {
    JournalEntry entry = {};
    entry.Load(journal_io_);

    if (entry.is_chunk) {
      entries.back().content += entry.content;
    } else {
      entries.push_back(std::move(entry));
    }

    pos += offset;
    journal_io_.seekg(pos);
  }

  CloseFileStream();

  return entries;
}

auto DiskManager::ReadEntry(int id) -> JournalEntry {
  OpenFileStream();

  int pos = id * JOURNAL_ENTRY_SIZE;

  journal_io_.seekg(pos);
  std::cout << "file size: " << std::filesystem::file_size("test.journal")
            << "\n";

  if (journal_io_.eof()) {
    throw std::runtime_error(std::format(
        "entry not found at expected position {} of journal file (EOF)\n",
        pos));
  }

  JournalEntry entry = {};
  entry.Load(journal_io_);

  if (entry.is_chunk) {
    // Entry was split into checks, check next entries for remaining chunks
    // auto following_chunk = ReadEntry(id + 1);
    // entry.content += following_chunk.content;

    pos += JOURNAL_ENTRY_SIZE;
    while (!journal_io_.eof()) {
      JournalEntry chunk = {};
      chunk.Load(journal_io_);

      if (chunk.is_chunk) {
        entry.content += chunk.content;
      } else {
        break;
      }
      pos += JOURNAL_ENTRY_SIZE;
    }
  }

  CloseFileStream();

  return entry;
}

auto DiskManager::OpenFileStream() -> void {
  // Open and check the file I/O stream
  journal_io_.open(filename_, std::ios::in | std::ios::out | std::ios::binary |
                                  std::ios::app);

  if (!journal_io_.is_open()) {
    throw std::runtime_error(
        std::format("Error opening file: {}\n", std::strerror(errno)));
  }
}

auto DiskManager::CloseFileStream() -> void {
  // Close the filestream, must be done to write ("flush") to a file
  journal_io_.close();
}
