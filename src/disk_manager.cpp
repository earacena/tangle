#include "include/disk_manager.h"

#include "include/journal_entry.h"

auto DiskManager::WriteEntry(JournalEntry &entry) -> void {
  OpenFileStream();

  entry.Store(journal_io_);

  CloseFileStream();
}

auto DiskManager::ReadEntries() -> std::vector<JournalEntry> {
  OpenFileStream();
  std::vector<JournalEntry> entries = {};
  journal_io_.seekg(0);

  // Cannot use journal_io_.eof() for this loop because eof() only checks if a
  // "read" was done after EOF, when we need to stop before reading EOF
  while (journal_io_.peek() != EOF) {
    JournalEntry entry = {};
    entry.Load(journal_io_);

    if (entries.size() > 0 && entry.is_chunk && entry.id == entries.back().id) {
      entries.back().content += entry.content;
    } else {
      entries.push_back(std::move(entry));
    }
  }

  CloseFileStream();

  return entries;
}

auto DiskManager::ReadEntry(int id) -> JournalEntry {
  OpenFileStream();

  journal_io_.seekg(0);

  auto entry_not_found = true;
  JournalEntry current_entry = {};
  while (journal_io_.peek() != EOF) {
    current_entry.Load(journal_io_);

    if (current_entry.id == id) {
      entry_not_found = false;
      break;
    }
  }

  if (entry_not_found) {
    throw std::runtime_error("error: entry not found, reached eof");
  }

  if (current_entry.is_chunk) {
    std::vector<std::string> chunk_contents = {};
    while (journal_io_.peek() != EOF) {
      JournalEntry chunk = {};
      chunk.Load(journal_io_);

      if (chunk.is_chunk && chunk.id == current_entry.id) {
        current_entry.content += chunk.content;
      }
    }
  }

  CloseFileStream();

  return current_entry;
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
