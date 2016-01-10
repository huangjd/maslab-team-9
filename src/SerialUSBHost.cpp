#include "SerialUSBHost.h"

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>

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

 // tty.c_cc[VMIN]  = 0;            // read doesn't block
//  tty.c_cc[VTIME] = 1;            // 0.5 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    return -1;
  }

  // TODO check CPUID

  return fd;
}

SerialUSBHost::SerialUSBHost() : seqNum(0), queueValid(0ull), device_fd(-1), defaultDevice{"/dev/ttyACM0"} {
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

int SerialUSBHost::send(size_t n, const char buf[]) {
  write(device_fd, buf, n);
}

int SerialUSBHost::recv(size_t n, char buf[]) {
  read(device_fd, buf, n);
}
