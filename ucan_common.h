#ifndef UCAN_COMMON_H
#define UCAN_COMMON_H

#include "ucan_commands_manager.h"
#include <chrono>
#include <deque>
#include <string>

class common_ucan_cmd {
public:
  Iucan_sendable send();
  string toString();
  common_ucan_cmd(std::chrono::milliseconds _timeout, int _count)
      : count(_count), timeout(_timeout) {}

protected:
  int count;
  std::chrono::milliseconds timeout;
};

template <class T> class ucan_device{
public:
  ucan_device(int _id) : id(_id) {}
  void execute() { this->mngr.start(this->command_queue, this->id); }
  int get_id() { return id; }
  void add(T command) {
    this->command_queue.push_back(command);
  }
  T get_command_in_queue(int i) { return command_queue[i]; }

private:
  int id;
  deque<T> command_queue;
  ucan_commands_manager<T> mngr;
};

#endif // UCAN_COMMON_H

