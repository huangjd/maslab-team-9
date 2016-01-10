#include "SerialUSBHost.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#undef ECHO
#include "../firmware/src/ProtocolConstants.h"

using namespace std;

typedef uint8_t u8;

static const int USBSpeed = B115200;

static int tryOpen(const char* deviceName) {
  int fd = open(deviceName, O_RDWR | O_NOCTTY | O_SYNC);
  if (fd < 0) {
    return -1;
  }

  termios tty;
  memset(&tty, 0, sizeof(tty));
  if (tcgetattr(fd, &tty) != 0) {
    return -1;
  }
  cfmakeraw(&tty);

  cfsetospeed(&tty, USBSpeed);
  cfsetispeed(&tty, USBSpeed);

  tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls, enable reading
  tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
  tty.c_cflag &= ~CSTOPB; // 1 stop bit

  //tty.c_iflag &= ~IGNBRK;         // disable break processing
//  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

  tty.c_lflag = 0;                // no signaling chars, no echo, no canonical processing
  tty.c_oflag = 0;                // no remapping, no delays

  tty.c_cc[VMIN]  = 0;            // read doesn't block
  tty.c_cc[VTIME] = 1;            // 0.1 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    return -1;
  }

  // TODO check CPUID

  return fd;
}

SerialUSBHost::SerialUSBHost() : device_fd(-1), defaultDevice{"/dev/ttyACM0"} {
  device_fd = tryOpen(defaultDevice);
  if (device_fd < 0) {
    for (char c = '1'; c <= '9'; c++) {
      defaultDevice[11] = c;
      device_fd = tryOpen(defaultDevice);
      if (device_fd >= 0) {
        return;
      }
    }

    perror("Unable to find teensy device!");
    throw exception();
  }
}

SerialUSBHost::~SerialUSBHost() {
  close(device_fd);
}

int SerialUSBHost::sendRaw(uint8_t c) {
  return write(device_fd, &c, 1);
}

int SerialUSBHost::sendRaw(const std::vector<uint8_t> &buf) {
  return write(device_fd, &buf[0], buf.size());
}

int SerialUSBHost::sendCmd(const std::vector<uint8_t> &buf) {
  if (buf.size() == 0) {
    return 0;
  }
  if (buf.size() > APPLICATION_MTU) {
    errno = ENOSPC;
    return -1;
  }

  outputBuf[0] = CMD;
  size_t n = 1;
  for (uint8_t c : buf) {
    if ((Characters) c < MAX_TRANSMISSION_CONTROL) {
      outputBuf[n++] = ESC;
      outputBuf[n++] = c + ESCAPE_OFF;
    } else {
      outputBuf[n++] = c;
    }
  }
  return write(device_fd, outputBuf, n);
}

int SerialUSBHost::sendCmd(size_t n, const char buf[]) {
  if (n == 0) {
    return 0;
  }
  if (n > APPLICATION_MTU) {
    errno = ENOSPC;
    return -1;
  }

  outputBuf[0] = CMD;
  size_t count = 1;
  for (int i = 0; i < n; i++) {
    if ((Characters) buf[i] < MAX_TRANSMISSION_CONTROL) {
      outputBuf[count++] = ESC;
      outputBuf[count++] = buf[i] + ESCAPE_OFF;
    } else {
      outputBuf[count++] = buf[i];
    }
  }
  return write(device_fd, outputBuf, count);
}

int SerialUSBHost::recvRaw(uint8_t *c) {
  return read(device_fd, c, 1);
}

/*int SerialUSBHost::recv(std::vector<uint8_t> &buf) {
  read(device_fd, buf, n);
}*/
