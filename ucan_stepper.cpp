#include "ucan_stepper.h"
#include <sstream>

ucan_stepper::ucan_stepper(int _id) : id(_id) {}

int ucan_stepper::get_id() { return id; }

string ucan_stepper::cmd::send() {
  std::ostringstream st;
  st << "Frame";
  st << this->p;
  st << this->n;
  return st.str();
};

ucan_stepper& ucan_stepper::add(ucan_stepper::cmd::name c, int stepNumber,
                  std::chrono::milliseconds timeount_ms)
{
    auto cmd = ucan_stepper::cmd(c,stepNumber,timeount_ms);
    this->command_queue.push_back(cmd);
    return *this;
}
