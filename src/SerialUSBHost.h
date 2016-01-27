#ifndef __SERIAL_USB_HOST_H__
#define __SERIAL_USB_HOST_H__

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <pthread.h>
#include <atomic>
#include <mutex>
#include <list>

class SerialUSBHost {
  int device_fd;
  char defaultDevice[13];

public:
  SerialUSBHost();
  ~SerialUSBHost();

  void sendRaw(const char *c, size_t size = 1);
  size_t recvRaw(char *c, size_t size = 1);

  void sendCmd(const std::string &s);
  void recvCmd(std::string &s);

  const int getfd() { // !!!
    return device_fd;
  }
};

class USBProxy {

  struct RecvCommand {
    volatile bool* available;
    const char* fmt;
    std::vector<void*> args;
  };

  SerialUSBHost host;
  pthread_t pid;

  std::atomic_bool alive;

  std::mutex lock;
  std::list<RecvCommand> cmdQueue;

  static void *USBProxyRoutine(void *arg);
public:

  USBProxy();
  ~USBProxy();

  void sendCmd(const std::string &s);
  void recvCmd(volatile bool *available, std::string *s);
  void recvCmd(volatile bool *available, const char* fmt, ...);
  void recvCmdBlocked(std::string *s);
  void recvCmdBlocked(const char* fmt, ...);
};

class USBProxy2 {
  struct Command {
    std::string cmd;
    volatile bool *ok;
    std::string *writeBack;
  };

  SerialUSBHost host;
  pthread_t pid;

  std::atomic_bool alive;

  std::mutex lock;
  std::list<Command> cmdQueue;

  static void *USBProxyRoutine(void *arg);
public:
  USBProxy2();
  ~USBProxy2();

  void sendCmd(const std::string &s, volatile bool *ok = nullptr);
  void sendCmd(const std::string &s, volatile bool *ok, std::string &response);

  std::string sendCmdBlocked(const std::string &s);
};

extern USBProxy2 usbProxy;

#endif // __SERIAL_USB_HOST_H__
