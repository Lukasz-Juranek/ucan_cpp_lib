#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "ucan_stepper.h"
#include <chrono>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST_CASE("uCAN Stepper driver creation", "[stepper]") {

  ucan_stepper s = ucan_stepper(7);
  REQUIRE(s.get_id() == 7);
}

TEST_CASE("uCAN Stepper command creatation", "[stepper]") {

  ucan_stepper::cmd c =
      ucan_stepper::cmd(ucan_stepper::cmd::rotate_anti_clockwhise, 10, 10ms);
  REQUIRE(c.toString() == "Frame101");
}

TEST_CASE("uCAN Stepper sending", "[stepper]") {
  ucan_stepper s = ucan_stepper(7);
  s.add(ucan_stepper::cmd::rotate_anti_clockwhise, 10, 10ms);
  s.add(ucan_stepper::cmd::rotate_anti_clockwhise, 12, 10ms);
  REQUIRE(s.get_command_in_queue(0).toString() == "Frame101");
  REQUIRE(s.get_command_in_queue(1).toString() == "Frame121");
}
