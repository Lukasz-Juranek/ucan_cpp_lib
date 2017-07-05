#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "ucan_stepper.h"
#include "ucan_line_motor.h"
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST_CASE("uCAN Stepper driver creation", "[stepper]") {
  ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(7);
  REQUIRE(s.get_id() == 7);
}

TEST_CASE("uCAN Stepper command creatation", "[stepper]") {
  ucan_stepper c =
      ucan_stepper(ucan_stepper::rotate_anti_clockwhise, 10, 10ms, 3);
  REQUIRE(c.toString() == "Stepper101");
}

TEST_CASE("uCAN Line motor command creatation", "[line_motor]") {
  ucan_line_motor c =
      ucan_line_motor(ucan_line_motor::motor_break, 10, 10ms, 3);
  REQUIRE(c.toString() == "Line102");
}


TEST_CASE("uCAN Stepper toString", "[stepper]") {
  ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(7);
  s.add(ucan_stepper(ucan_stepper::rotate_anti_clockwhise, 10, 10ms, 1));
  s.add(ucan_stepper(ucan_stepper::rotate_anti_clockwhise, 12, 10ms, 1));
  REQUIRE(s.get_command_in_queue(0).toString() == "Stepper101");
  REQUIRE(s.get_command_in_queue(1).toString() == "Stepper121");
}

TEST_CASE("uCAN Stepper executing commands", "[stepper]") {
  ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(7);
  s.add(ucan_stepper(ucan_stepper::rotate_anti_clockwhise, 10, 10ms, 3));
  s.add(ucan_stepper(ucan_stepper::rotate_anti_clockwhise, 14, 100ms, 5));

  ucan_device<ucan_line_motor> l = ucan_device<ucan_line_motor>(2);
  l.add(ucan_line_motor(ucan_line_motor::rotate_clockwhise, 7, 150ms, 3));

  s.execute();
  l.execute();

  std::this_thread::sleep_for(1s);
}
