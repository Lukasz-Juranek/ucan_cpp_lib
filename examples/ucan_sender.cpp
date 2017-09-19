#include "./../src/json.hpp"
#include "./../src/ucan_line_motor.h"
#include "./../src/ucan_stepper.h"
#include "./../src/ucan_tools.h"
#include <chrono>
#include <iostream>
#include <limits.h>
#include <linux/can.h>
#include <thread>

using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;
using json = nlohmann::json;

ucan_stepper::CANStatusFrame1 status1;
uCANnetID status_id;
int counter = 0;

void test_callback_function(can_frame *buffer) {
  memcpy(&status1, buffer->data, sizeof(CAN_MAX_DLEN));
  status_id.whole = buffer->can_id;
  counter++;
}

int main(int argc,     // Number of strings in array argv
         char *argv[], // Array of command-line argument strings
         char *envp[]) // Array of environment variable strings
{
  errno = 0;    
  char *p;
  const char *scan_interface = argv[1];
  long scan_len = strtol(argv[2], &p, 10);


  if (argc != 3 || errno != 0 || *p != '\0' || scan_len > INT_MAX) {
    std::cout << "uCAN devices send utility, output is JSON format"
              << std::endl;
    std::cout << "usage: ucan_sender CAN_INTERFACE JSON_COMMAND"
              << std::endl;
    std::cout << "example: ucan_sender vcan {\"id\": \"22\", \"type\": \"STEPPER_MOTOR\", \" dir: ...}" << std::endl;
  } else {

    ucan_can_interface::set_interface_name(scan_interface);
    can_frame buffer;
    ucan_can_interface can_sock = ucan_can_interface(ucan_can_interface::interface_name);

    json inpuut = "{ \"happy\": true, \"pi\": 3.141 }"_json;

    switch (device_type)
    case "STEPPER_MOTOR":
    case "LINE_MOTOR"

    CANBrushedCMD1 cmd;
    memcpy(&cmd,buffer.data,CAN_MAX_DLEN);

    can_send(buffer);

    CANStepperCMD1 cmd;

  }
}

