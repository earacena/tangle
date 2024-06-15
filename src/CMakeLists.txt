add_library(
  tangle
  journal.cpp
  argument_parser.cpp
  include/journal.h
  include/argument_parser.h
)

target_compile_features(tangle PUBLIC cxx_std_20)
target_include_directories(tangle PUBLIC include)
