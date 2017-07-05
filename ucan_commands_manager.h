#ifndef UCAN_COMMANDS_MANAGER_H
#define UCAN_COMMANDS_MANAGER_H

#include "ucan_common.h"
#include <boost/log/trivial.hpp>
#include <chrono>
#include <deque>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

class Iucan_sendable {
public:
  Iucan_sendable(string _data, std::chrono::milliseconds _timeout,
                 int _repeat_count)
      : data(_data), timeout(_timeout), repeat_count(_repeat_count) {}
  string toString() { return data; }
  std::chrono::milliseconds get_timeout() { return timeout; }
  bool command_expired() {
    if (repeat_count > 0) {
      repeat_count--;
    }
    if (repeat_count == 0)
      return true;
    return false;
  }

private:
  string data;
  std::chrono::milliseconds timeout;
  int repeat_count;
};

template <class T> class ucan_commands_manager {
private:
  uCANnetID device_id;
  thread send_thread;
  static void manage_commands(uCANnetID device_id,
                              deque<Iucan_sendable> command_queue);

public:
  ucan_commands_manager() {}
  void start(deque<T> t_command, uCANnetID _device_id);
  void stop() {
    if (this->send_thread
            .joinable()) // wait for thread to finish befone making new one
    {
      this->send_thread.join();
    }
  }

  ~ucan_commands_manager() { this->stop(); }
  ucan_commands_manager(ucan_commands_manager &&) = default;
};

template <class T>
void ucan_commands_manager<T>::start(deque<T> t_command, uCANnetID _device_id) {

  this->device_id = _device_id;
  BOOST_LOG_TRIVIAL(trace) << "ucan_command_manager execute ";

  // stop previouse thread before making new one
  this->stop();

  // generate queue for manager
  deque<Iucan_sendable> command_queue;
  for (T i : t_command) {
    command_queue.push_back(i.send());
  }
  // start sending thread
  thread t(ucan_commands_manager::manage_commands, this->device_id,
           command_queue);
  send_thread = std::move(t);
}

template <class T>
void ucan_commands_manager<T>::manage_commands(
    uCANnetID device_id, deque<Iucan_sendable> command_queue) {
  while (command_queue.size() > 0) {

    auto f = &command_queue[0];
    if (f->command_expired()) {
      command_queue.pop_front();
    }

    std::this_thread::sleep_for(f->get_timeout());
    BOOST_LOG_TRIVIAL(trace) << std::hex << (int)device_id.whole << " : " << f->toString();
  }
}

#endif // UCAN_COMMANDS_MANAGER_H
