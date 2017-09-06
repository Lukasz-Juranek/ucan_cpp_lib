
#include "./../src/ucan_line_motor.h"
#include "./../src/ucan_stepper.h"
#include "./../src/ucan_tools.h"
#include <chrono>
#include <iostream>
#include <linux/can.h>
#include <thread>
#include <limits.h>
#include <vector>
#include <cstdlib>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;

void callback_function(can_frame *buffer) {

  uCANnetID status_id;
  status_id.whole = buffer->can_id;

  std::cout << "{" << std::endl;
  std::cout << "{" << std::endl;
  std::cout << "\"id\" : \"" << status_id.id << "\"";
  std::cout << "\"device_type\" : \"" << status_id.type << "\"";
  std::cout << "\"frame_type\" : \"" << status_id.frame_type << "\"";
  std::cout << "\"group\" : \"" << status_id.group << "\"";
  std::cout << std::endl << "}," << std::endl;

  switch (status_id.type) {
    case  ucan_stepper::driver_id:
    {
        ucan_stepper::CANStatusFrame1 s1;
        memcpy(&s1, buffer->data, sizeof(CAN_MAX_DLEN));
        std::cout << "{" << std::endl;
        std::cout << "\"sensors.Position\" : \"" << s1.sensors.Position << "\"";
        std::cout << "\"sensors.Speed\" : \"" << s1.sensors.Speed<< "\"";
        std::cout << "\"stepper.StepCount\" : \"" << s1.stepper.StepCount<< "\"";
        std::cout << "\"stepper.nowStepping\" : \"" << s1.stepper.nowStepping<< "\"";
        std::cout << "}" << std::endl;
        break;
    }

    case ucan_line_motor::driver_id:
    {
      ucan_line_motor::CANStatusFrame1 s1;
      memcpy(&s1, buffer->data, sizeof(CAN_MAX_DLEN));
      std::cout << "{" << std::endl;
      std::cout << "\"sensors.Position\" : \"" << s1.sensors.Position << "\"";
      std::cout << "\"sensors.Speed\" : \"" << s1.sensors.Speed<< "\"";
      std::cout << "\"brushed.dir\" : \"" << s1.brushed.dir<< "\"";
      std::cout << "\"brushed.pwmValue\" : \"" << s1.brushed.pwmValue<< "\"";
      std::cout << "\"brushed.state\" : \"" << s1.brushed.state<< "\"";
      std::cout << "}" << std::endl;
      break;
    }

    default:
      break;
  }
  std::cout << "}" << std::endl;
}

std::vector<ucan_device<ucan_stepper>*>steppers;
std::vector<ucan_device<ucan_line_motor>*> line;


void exiting() {
    std::cout << "Exiting";

    for(auto n : steppers) {
        free(n);
    }

    for(auto n : line) {
        free(n);
    }
}

int main(int argc,     // Number of strings in array argv
         char *argv[], // Array of command-line argument strings
         char *envp[]) // Array of environment variable strings
{
  errno = 0;
  char *p;
  const char *scan_interface = argv[1];


  if (argc > 2) {
      std::cout << "uCAN devices reception utility, output is JSON format" << std::endl;
      std::cout << "usage: ucan_receiver CAN_INTERFACE UCAN_ID UCAN_TYPE ... [UCAN_TYPE can be STEPPER_MOTOR, LINE_MOTOR] " << std::endl;
      std::cout << "example: ucan_receiver vcan0 A7 STEPPER_MOTOR" << std::endl;
      std::cout << "output: { {CAN_ID, data1, data2}," << std::endl;
      std::cout << " {CAN_ID, data1, data2}}" << std::endl;
  } else {

      std::atexit(exiting);

      ucan_can_interface::set_interface_name(scan_interface);

      long can_id = strtol(argv[2], &p, 10);
      const char *device_type = argv[3];
      int deviceType = ucan_line_motor::driver_id;


      if (strcmp(device_type, "STEPPER_MOTOR") == 0) {
          ucan_device<ucan_stepper> * s = new ucan_device<ucan_stepper>(can_id);
          s->recive_frame(callback_function, ucan_stepper::driver_id);
          steppers.push_back(s);
      }
      if (strcmp(device_type, "LINE_MOTOR") == 0) {
          ucan_device<ucan_line_motor> * s = new ucan_device<ucan_line_motor>(can_id);
          s->recive_frame(callback_function, ucan_line_motor::driver_id);
          line.push_back(s);
      }

      while (1)
      {
        std::this_thread::sleep_for(10s);
      }
  }
}



