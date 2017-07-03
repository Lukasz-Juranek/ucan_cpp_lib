#ifndef UCAN_COMMANDS_MANAGER_H
#define UCAN_COMMANDS_MANAGER_H

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

private:
  string data;
  std::chrono::milliseconds timeout;
  int repeat_count;
};

template <class T> class ucan_commands_manager {
private:
  thread t;
  deque<Iucan_sendable> command_queue;

public:
  ucan_commands_manager() {};
  void start(deque<T> t_command);
  void stop();
  //  ~ucan_commands_manager() {this->stop();};
};

template <class T> void ucan_commands_manager<T>::start(deque<T> t_command) {
  for (T i : t_command) {
    this->command_queue.push_back(i.send());

    for (auto i : this->command_queue) {
      cout << i.toString() << endl;
    }
  }
}

#endif // UCAN_COMMANDS_MANAGER_H
