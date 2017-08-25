#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
                          // in one cpp file
#include "catch.hpp"
#include "ucan_line_motor.h"
#include "ucan_stepper.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <linux/can.h>


using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

TEST_CASE("uCAN Stepper driver creation", "[stepper]") {
  ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(7);
  REQUIRE(s.get_id().id == 7);
}

TEST_CASE("uCAN Stepper command creatation", "[stepper]") {
  ucan_stepper c = ucan_stepper(
      {ucan_stepper::micro16, ucan_stepper::rotate_clockwhise, 0, 254}, 10ms, 3);
  REQUIRE(c.toString() == "04010000fe000000");
}

TEST_CASE("uCAN Line motor command creatation", "[line_motor]") {

  ucan_line_motor::CANBrushedCMD1 cmd{{100, 0}};
  ucan_line_motor c = ucan_line_motor(cmd, 10ms, 3);
  REQUIRE(c.toString() == "6400000000000000");
}
TEST_CASE("uCAN Stepper toString", "[stepper]") {
  ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(7);

  ucan_stepper::CANStepperCMD1 cmd{
      {ucan_stepper::micro16, ucan_stepper::rotate_clockwhise, 0, 100}};

  s.add(ucan_stepper(cmd, 10ms, 1));

  REQUIRE(s.get_command_in_queue(0).toString() == "0401000064000000");
}

TEST_CASE("uCAN Stepper executing commands", "[stepper]") {
  ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(7);
  s.add(ucan_stepper(
      {ucan_stepper::micro16, ucan_stepper::rotate_clockwhise, 0, 14}, 10ms,
      3));
  s.add(ucan_stepper(
      {ucan_stepper::micro16, ucan_stepper::rotate_clockwhise, 0, 15}, 100ms,
      5));

  ucan_device<ucan_line_motor> l = ucan_device<ucan_line_motor>(7);
  l.add(ucan_line_motor({100, 0}, 150ms, 3));

  s.execute();
  l.execute();

  std::this_thread::sleep_for(1s);
}

TEST_CASE("Recive CAN Frame", "[rx]"){
   ucan_device<ucan_stepper> s = ucan_device<ucan_stepper>(15);
   can_frame c;
   auto id = s.get_id();
   s.recive_frame(&c, ucan_stepper::status_frame_id);

   ucan_stepper::CANStatusFrame1 status1;

   std::this_thread::sleep_for(1s);
   memcpy(&status1,c.data,sizeof(CAN_MAX_DLEN));
   printf("CAN_ID %08x \n\r", c.can_id);
   printf("STEPPER_SPEED %08x, POSITION %08x \n\r", status1.sensors.Speed, status1.sensors.Position);
   std::this_thread::sleep_for(1s);
}
