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
    cmd(ucan_stepper::cmd::name command_name, int parameter,
        std::chrono::milliseconds timeout)
        : n(command_name), p(parameter), t(timeout) {}

  private:
    name n;
    int p;
    std::chrono::milliseconds t;
  };

  ucan_stepper(int id);
  void execute();
  int get_id();
  ucan_stepper &add(ucan_stepper::cmd::name c, int stepNumber,
                    std::chrono::milliseconds timeount_ms);
  cmd get_command_in_queue(int i) { return command_queue[i]; };

private:
  int id;
  deque<ucan_stepper::cmd> command_queue;
  ucan_commands_manager<ucan_stepper::cmd> mngr;
};

#endif // UCAN_STEPPER_H
