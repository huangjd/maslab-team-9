#include "SerialUSBHost.h"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#undef ECHO

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
  tty.c_cc[VTIME] = 1;            // 0 seconds read timeout

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

void SerialUSBHost::sendRaw(const char *c, size_t size) {
  write(device_fd, c, size);
}

size_t SerialUSBHost::recvRaw(char *c, size_t size) {
  return read(device_fd, c, size);
}

void SerialUSBHost::sendCmd(const string &s) {
  string buf;
  buf.push_back('\x01');
  buf.insert(buf.end(), s.begin(), s.end());
  buf.push_back('\0');
  write(device_fd, &buf[0], buf.size());
}

void SerialUSBHost::recvCmd(string &s) {
  char c;
  read(device_fd, &c, 1);
  s.resize((size_t)c);
  read(device_fd, &s[0], (size_t)c);
}
