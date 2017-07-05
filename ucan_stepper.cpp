#include "ucan_stepper.h"
#include <memory>
#include <sstream>

using namespace std;

ucan_stepper::ucan_stepper(int _id) : id(_id) {}

int ucan_stepper::get_id() { return id; }

void ucan_stepper::execute() { this->mngr.start(this->command_queue); }

Iucan_sendable ucan_stepper::cmd::send() {
  return Iucan_sendable(this->toString(), this->timeout, this->count);
}

string ucan_stepper::cmd::toString() {
  std::ostringstream st;
  st << "Frame";
  st << this->parameter;
  st << this->command_name;
  return st.str();
};

ucan_stepper &ucan_stepper::add(ucan_stepper::cmd command) {
  this->command_queue.push_back(command);
  return *this;
}
