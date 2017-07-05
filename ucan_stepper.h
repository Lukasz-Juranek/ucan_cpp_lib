#ifndef UCAN_STEPPER_H
#define UCAN_STEPPER_H

#include "ucan_commands_manager.h"
#include <chrono>
#include <deque>
#include <string>

class common_cmd {
public:
  Iucan_sendable send();
  string toString();
  common_cmd(std::chrono::milliseconds _timeout, int _count)
      : count(_count), timeout(_timeout){};

protected:
  int count;
  std::chrono::milliseconds timeout;
};

class ucan_stepper {

public:
  class cmd : public common_cmd {
  public:
    enum name {
      rotate_clockwhise,
      rotate_anti_clockwhise,
    };

    cmd(ucan_stepper::cmd::name _command_name, int _parameter,
        std::chrono::milliseconds _timeout, int _count)
        : parameter(_parameter), command_name(_command_name),
          common_cmd(_timeout, _count) {}

    Iucan_sendable send();
    string toString();

  private:
    name command_name;
    int parameter;
  };

public:
  ucan_stepper(int id);
  void execute();
  int get_id();
  ucan_stepper &add(ucan_stepper::cmd command);
  cmd get_command_in_queue(int i) { return command_queue[i]; };

private:
  int id;
  deque<ucan_stepper::cmd> command_queue;
  ucan_commands_manager<ucan_stepper::cmd> mngr;
};

#endif // UCAN_STEPPER_H
