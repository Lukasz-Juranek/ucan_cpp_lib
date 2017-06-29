#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include <iostream>

using namespace std;

TEST_CASE("Factorials are computed", "[factorial]") { REQUIRE((1 + 2) == 1); }
