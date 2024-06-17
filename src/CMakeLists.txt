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

target_compile_features(journal PUBLIC cxx_std_20)
target_include_directories(journal PUBLIC include)
target_compile_features(argparse PUBLIC cxx_std_20)
target_include_directories(argparse PUBLIC include)
