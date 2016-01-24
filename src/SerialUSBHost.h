#ifndef __SERIAL_USB_HOST_H__
#define __SERIAL_USB_HOST_H__

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

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

#endif // __SERIAL_USB_HOST_H__
