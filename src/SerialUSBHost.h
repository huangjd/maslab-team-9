#ifndef __SERIAL_USB_HOST_H__
#define __SERIAL_USB_HOST_H__

#include <cstddef>
#include <cstdint>
#include <vector>

class SerialUSBHost {
  int device_fd;

  char defaultDevice[13];

  static constexpr size_t OUTPUT_BUFFER_MTU = 520;
  static constexpr size_t APPLICATION_MTU = 259;

  uint8_t outputBuf[OUTPUT_BUFFER_MTU];

public:
  SerialUSBHost();
  ~SerialUSBHost();

  int sendRaw(uint8_t c);
  int sendRaw(const std::vector<uint8_t> &buf);

  int sendCmd(const std::vector<uint8_t> &buf);
  int sendCmd(size_t n, const char buf[]);

  int recvRaw(uint8_t *c);

//  int recv(std::vector<uint8_t> &buf);
};

#endif // __SERIAL_USB_HOST_H__
