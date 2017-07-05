#ifndef UCAN_STEPPER_H
#define UCAN_STEPPER_H

#include "ucan_common.h"
#include <chrono>
#include <deque>
#include <memory>
#include <sstream>
#include <string>

// command set for stepper motor
class ucan_stepper : public common_ucan_cmd {
public:
  enum name {
    rotate_clockwhise,
    rotate_anti_clockwhise,
  };

  ucan_stepper(name _command_name, int _steps,
               std::chrono::milliseconds _timeout, int _count)
      : steps(_steps), command_name(_command_name),
        common_ucan_cmd(_timeout, _count) {}

  Iucan_sendable send() {
    return Iucan_sendable(this->toString(), this->timeout, this->count);
  }
  string toString() {
    std::ostringstream st;
    st << "Stepper";
    st << this->steps;
    st << this->command_name;
    return st.str();
  }

private:
  name command_name;
  int steps;
};

#endif // UCAN_STEPPER_H
