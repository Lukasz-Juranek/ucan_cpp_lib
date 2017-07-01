#include "ucan_stepper.h"

ucan_stepper::ucan_stepper(int _id) : id(_id) {}

int ucan_stepper::get_id() { return id; }

//string ucan_stepper::send() {return };
