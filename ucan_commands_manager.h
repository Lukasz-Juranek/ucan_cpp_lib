#ifndef UCAN_COMMANDS_MANAGER_H
#define UCAN_COMMANDS_MANAGER_H

#include <string>

using namespace std;

class Iucan_sendable {
public:
  virtual string send(){return "error";};
};

class ucan_commands_manager
{
public:
    ucan_commands_manager();
};

#endif // UCAN_COMMANDS_MANAGER_H
