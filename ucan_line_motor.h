#ifndef UCAN_LINE_MOTOR_H
#define UCAN_LINE_MOTOR_H

#include "ucan_common.h"

class ucan_line_motor : public common_ucan_cmd {
public:
  enum name {
    rotate_clockwhise,
    rotate_anti_clockwhise,
    motor_break,
  };

  ucan_line_motor(name _command_name, int _speed,
                 std::chrono::milliseconds _timeout, int _count)
      : speed(_speed), command_name(_command_name),
        common_ucan_cmd(_timeout, _count) {}

  Iucan_sendable send() {
    return Iucan_sendable(this->toString(), this->timeout, this->count);
  }
  string toString() {
    std::ostringstream st;
    st << "Line";
    st << this->speed;
    st << this->command_name;
    return st.str();
  }

private:
  name command_name;
  int speed;
};

#endif // UCAN_LINE_MOTOR_H
