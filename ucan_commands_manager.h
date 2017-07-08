#ifndef UCAN_COMMANDS_MANAGER_H
#define UCAN_COMMANDS_MANAGER_H

#include "ucan_can_interface.h"
#include "ucan_common.h"
#include <boost/log/trivial.hpp>
#include <chrono>
#include <deque>
#include <iostream>
#include <linux/can.h>
#include <string>
#include <thread>

class Iucan_sendable {
public:
  Iucan_sendable(can_frame _data, std::chrono::milliseconds _timeout,
                 int _repeat_count)
      : data(_data), timeout(_timeout), repeat_count(_repeat_count) {}

  std::string toString() {
    std::ostringstream st;
    for (int i = 0; i < sizeof(data.can_dlc); ++i) {
      auto tmp = data.data[i];
      st << std::setfill('0') << std::setw(2) << std::hex << (int)tmp;
    }
    return st.str();
  }

  std::chrono::milliseconds get_timeout() { return timeout; }
  bool command_expired() {
    if (repeat_count > 0) {
      repeat_count--;
    }
    if (repeat_count == 0)
      return true;
    return false;
  }

  can_frame get_can_frame() { return data; }

private:
  can_frame data;
  std::chrono::milliseconds timeout;
  int repeat_count;
};

template <class T> class ucan_commands_manager {
private:
  uCANnetID device_id;
  std::thread send_thread;
  static void manage_commands(uCANnetID device_id,
                              std::deque<Iucan_sendable> command_queue);

public:
  ucan_commands_manager() {}
  void start(std::deque<T> t_command, uCANnetID _device_id);
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
void ucan_commands_manager<T>::start(std::deque<T> t_command,
                                     uCANnetID _device_id) {

  this->device_id = _device_id;
  BOOST_LOG_TRIVIAL(trace) << "ucan_command_manager execute " << std::hex
                           << (int)this->device_id.id;

  // stop previouse thread before making new one
  this->stop();

  // generate queue for manager
  std::deque<Iucan_sendable> command_queue;
  for (T i : t_command) {
    command_queue.push_back(i.send());
  }
  // start sending thread
  std::thread t(ucan_commands_manager::manage_commands, this->device_id,
                command_queue);
  send_thread = std::move(t);
}

template <class T>
void ucan_commands_manager<T>::manage_commands(
    uCANnetID device_id, std::deque<Iucan_sendable> command_queue) {

  ucan_can_interface can_sock = ucan_can_interface("vcan0");
  while (command_queue.size() > 0) {

    auto f = command_queue[0];
    if (command_queue[0].command_expired()) {
      command_queue.pop_front();
    }

    std::this_thread::sleep_for(f.get_timeout());

    can_frame frame = f.get_can_frame();
    frame.can_id = device_id.get_whole();

    can_sock.can_send(frame);
    BOOST_LOG_TRIVIAL(trace) << device_id.toString() << ":" << f.toString();
  }
}

#endif // UCAN_COMMANDS_MANAGER_H
