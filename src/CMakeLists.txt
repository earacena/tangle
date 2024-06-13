add_library(
  tangle
  journal.cpp
  include/journal.h
)

target_compile_features(tangle PUBLIC cxx_std_20)
target_include_directories(tangle PUBLIC include)
