
cmake_minimum_required(VERSION 3.18...3.27)
project(
  tangle
  VERSION 0.1.0
  DESCRIPTION "A commandline daily journaling tool written in C++20."
  LANGUAGES CXX
)

# Main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

  # Ensure -std=c++20 instead of -std=g++20
  set(CMAKE_CXX_EXTENSIONS OFF)
  set(CMAKE_CXX_STANDARD 20)

  # Enable IDE folder support
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Add testing library (Catch2)
Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# Add tests
add_executable(journal_test tests/journal_test.cpp src/journal.cpp)
add_executable(argument_parser_test tests/argument_parser_test.cpp src/argument_parser.cpp)
target_include_directories(journal_test PUBLIC src/include)
target_include_directories(argument_parser_test PUBLIC src/include)
target_link_libraries(journal_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(argument_parser_test PRIVATE Catch2::Catch2WithMain)

# Add source code directory
add_subdirectory(src)

# Add executable directory
# add_subdirectory(apps)
