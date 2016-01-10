#ifndef __SERIAL_USB_HOST_H__
#define __SERIAL_USB_HOST_H__

#include <cstddef>
#include <cstdint>
#include <vector>

class SerialUSBHost {
  uint8_t seqNum;
  int device_fd;

  std::vector<uint8_t> msgQueue[64];
  uint64_t queueValid;

  char defaultDevice[13];

public:
  SerialUSBHost();
  ~SerialUSBHost();

  int send(size_t n, const char buf[]);
  int recv(size_t n, char buf[]);
};

#endif // __SERIAL_USB_HOST_H__
