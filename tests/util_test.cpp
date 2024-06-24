#include "../src/include/util.h"

#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Correctly parses seconds from epoch (UNIX timestamp) into string",
          "[date]") {
  std::chrono::system_clock::rep seconds = 1719246515;

  auto date_string = SecondsToDateString(seconds);

  REQUIRE(date_string.compare("2024-06-24 12:28:35") == 0);
}
