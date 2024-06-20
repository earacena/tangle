
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
add_executable(disk_manager_test tests/disk_manager_test.cpp src/disk_manager.cpp src/journal_entry.cpp)
add_executable(argument_parser_test tests/argument_parser_test.cpp src/argument_parser.cpp src/journal.cpp src/journal_entry.cpp src/disk_manager.cpp)
add_executable(command_test tests/command_test.cpp src/argument_parser.cpp src/journal.cpp src/disk_manager.cpp src/journal_entry.cpp)
add_executable(journal_entry_test tests/journal_entry_test.cpp src/journal_entry.cpp)

target_include_directories(disk_manager_test PUBLIC src/include)
target_include_directories(argument_parser_test PUBLIC src/include)
target_include_directories(command_test PUBLIC src/include)
target_include_directories(journal_entry_test PUBLIC src/include)

target_link_libraries(disk_manager_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(argument_parser_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(command_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(journal_entry_test PRIVATE Catch2::Catch2WithMain)

# Add source code directory
add_subdirectory(src)

# Add executable directory
add_subdirectory(apps)
