#ifndef UCAN_STEPPER_H
#define UCAN_STEPPER_H

#include "ucan_commands_manager.h"
#include <chrono>
#include <deque>
#include <string>

class ucan_stepper {

public:
  class cmd {
  public:
    Iucan_sendable send();
    string toString();
    enum name {
      rotate_clockwhise,
      rotate_anti_clockwhise,
    };
    cmd(ucan_stepper::cmd::name _command_name, int _parameter,
        std::chrono::milliseconds _timeout, int _count)
        : command_name(_command_name), parameter(_parameter), timeout(_timeout),
          count(_count) {}

  private:
    name command_name;
    int parameter;
    int count;
    std::chrono::milliseconds timeout;
  };

  ucan_stepper(int id);
  void execute();
  int get_id();
  ucan_stepper &add(ucan_stepper::cmd::name command_name, int number_of_steps,
                    std::chrono::milliseconds timeount_ms, int count);
  cmd get_command_in_queue(int i) { return command_queue[i]; };

private:
  int id;
  deque<ucan_stepper::cmd> command_queue;
  ucan_commands_manager<ucan_stepper::cmd> mngr;
};

#endif // UCAN_STEPPER_H
