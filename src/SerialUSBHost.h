#ifndef __SERIAL_USB_HOST_H__
#define __SERIAL_USB_HOST_H__

#include <cstddef>
#include <cstdint>
#include <vector>

class SerialUSBHost {
  int device_fd;

  char defaultDevice[13];

  static constexpr size_t SEND_BUFFER_MTU = 520;
  static constexpr size_t RECV_BUFFER_MTU = 521;
  static constexpr size_t APPLICATION_MTU = 259;

  uint8_t sendBuf[SEND_BUFFER_MTU];

  uint8_t recvBuf[RECV_BUFFER_MTU];
  size_t  recvBufPtrLower, recvBufPtrUpper;

  bool escaped;

  int fetchRecvBuf();

public:
  SerialUSBHost();
  ~SerialUSBHost();

  int sendRaw(uint8_t c);
  int sendRaw(const std::vector<uint8_t> &buf);

  int sendCmd(const std::vector<uint8_t> &buf);
  int sendCmd(size_t n, const char buf[]);

  int recvRaw(uint8_t *c);
  int recvEscaped(uint8_t *c);
  int recvResponse(uint8_t *cmdtype, std::vector<uint8_t> &buf);
};

#endif // __SERIAL_USB_HOST_H__
