#ifndef UCAN_STEPPER_H
#define UCAN_STEPPER_H

#include <chrono>
#include <deque>
#include <string>

#include "ucan_commands_manager.h"

using namespace std;
using namespace std::chrono;

class ucan_stepper {

public:
  class cmd /*: public Iucan_sendable */ {
  public:
    enum name {
      rotate_clockwhise,
      rotate_anti_clockwhise,
    };
    cmd(ucan_stepper::cmd::name command_name, int parameter,
        milliseconds timeout)
        : n(command_name), p(parameter), t(timeout) {}

  private:
    name n;
    int p;
    milliseconds t;
  };

  ucan_stepper(int id);
  int get_id();
  ucan_stepper &add(ucan_stepper::cmd::name c, int stepNumber,
                    milliseconds timeount_ms);
  ucan_stepper &add(ucan_stepper::cmd::name c, double angle,
                    milliseconds timeount_ms);

private:
  int id;
  deque<ucan_stepper::cmd> command_queue;
};

#endif // UCAN_STEPPER_H
