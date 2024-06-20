add_library(
  journal
  journal.cpp
  include/journal.h
)

add_library(
  argparse
  argument_parser.cpp
  include/argument_parser.h
)

add_library(
  disk_manager
  disk_manager.cpp
  journal_entry.cpp
  include/disk_manager.h
  include/journal_entry.h
)

add_library(
  journal_entry
  journal_entry.cpp
  include/journal_entry.h
)

target_compile_features(journal PUBLIC cxx_std_20)
target_include_directories(journal PUBLIC include)

target_compile_features(argparse PUBLIC cxx_std_20)
target_include_directories(argparse PUBLIC include)

target_compile_features(journal_entry PUBLIC cxx_std_20)
target_include_directories(journal_entry PUBLIC include)

target_compile_features(disk_manager PUBLIC cxx_std_20)
target_include_directories(disk_manager PUBLIC include)

