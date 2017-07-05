#ifndef UCAN_COMMON_H
#define UCAN_COMMON_H

#include "ucan_net.h"
#include "ucan_commands_manager.h"
#include <chrono>
#include <deque>
#include <string>

// ------------ device types ------------------

#define MOTOR_DRIVER_ID_STEPPER 5
#define MOTOR_DRIVER_ID_LINE 6

// ----------------- frames types ------------------
#define MOTOR_CONTROL_FRAME_ID 13
#define STEPPER_STEP_CMD__ID 64


class common_ucan_cmd {
public:
  Iucan_sendable send();
  string toString();
  common_ucan_cmd(std::chrono::milliseconds _timeout, int _count)
      : count(_count), timeout(_timeout) {}

protected:
  int count;
  std::chrono::milliseconds timeout;
};

template <class T> class ucan_device{
public:
    ucan_device(int device_id) : ucan_net_id({T::driver_id, T::command_id, device_id, 0 ,0, 0}) {}
  void execute() { this->mngr.start(this->command_queue, this->ucan_net_id); }
  int get_id() { return ucan_net_id.id; }
  void add(T command) {
    this->command_queue.push_back(command);
  }
  T get_command_in_queue(int i) { return command_queue[i]; }

private:
  uCANnetID ucan_net_id;
  deque<T> command_queue;
  ucan_commands_manager<T> mngr;
};

#endif // UCAN_COMMON_H

