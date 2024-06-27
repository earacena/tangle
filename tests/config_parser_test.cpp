#include "../src/include/config_parser.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <iostream>

TEST_CASE("Parses env var correctly", "[env]") {
  // Test env
  // set $TEST to /env/path
  setenv("TEST", "~/test/path", 0);
  std::cout << "setting TEST to ~/test/path\n";

  // set $VALUE to 250
  setenv("VALUE", "250", 0);
  std::cout << "setting VALUE to 250\n";

  auto cfgparse = ConfigParser();

  auto config = cfgparse.Parse({{"test_path", "TEST", "~/test/fallback/path"},
                                {"env_value", "VALUE", "10"}});

  REQUIRE(config.at("test_path").size() != 0);
  REQUIRE(config.at("test_path").compare("~/test/path") == 0);
  REQUIRE(config.at("env_value") == "250");

  unsetenv("TEST");
  unsetenv("VALUE");
}

TEST_CASE("Returns fallback when env var is not set", "[env]") {
  // Dont set $TEST and $VALUE to test fallback
  unsetenv("TEST");
  unsetenv("VALUE");

  auto cfgparse = ConfigParser();

  auto config = cfgparse.Parse({{"test_path", "TEST", "~/test/fallback/path"},
                                {"env_value", "VALUE", "10"}});

  REQUIRE(config.at("test_path").size() != 0);
  REQUIRE(config.at("test_path").compare("~/test/fallback/path") == 0);
  REQUIRE(config.at("env_value").compare("10") == 0);
}
