#include "SerialUSBHost.h"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>

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
  tty.c_cc[VTIME] = 0;            // 0 seconds read timeout

  if (tcsetattr (fd, TCSANOW, &tty) != 0) {
    return -1;
  }

  // TODO check CPUID

  return fd;
}

SerialUSBHost::SerialUSBHost() : device_fd(-1), defaultDevice{"/dev/ttyACM0"} {
  device_fd = tryOpen(defaultDevice);
  while (device_fd < 0) {
    for (char c = '0'; c <= '9'; c++) {
      defaultDevice[11] = c;
      device_fd = tryOpen(defaultDevice);
      if (device_fd >= 0) {
        return;
      }
    }
    perror("Unable to find teensy device!");
    usleep(100000);
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
  fsync(device_fd);
}

void SerialUSBHost::recvCmd(string &s) {
  char c[2];
  char length = 0;
  while (true) {
    c[0] = '\0';
    c[1] = '\0';
    read(device_fd, c, 2);
    if (c[0] == '\x01') {
      length = c[1];
      break;
    } else if (c[1] == '\x01') {
      read(device_fd, &length, 1);
      break;
    }
  }
  s.resize(length + 2);
  read(device_fd, &s[0], length + 2);
  s.pop_back();
  s.pop_back();
}

void* USBProxy::USBProxyRoutine(void *arg) {
  USBProxy* self = reinterpret_cast<USBProxy*>(arg);
  while (self->alive) {
    bool hasCmd = false;
    RecvCommand cmd;

    self->lock.lock();
    if (self->cmdQueue.size()) {
      cmd = self->cmdQueue.front();
      self->cmdQueue.pop_front();
      hasCmd = true;
    }
    self->lock.unlock();

    if (hasCmd) {
      string s;
      self->host.recvCmd(s);
      if (cmd.fmt) {
        cmd.args.resize(10);
        sscanf(&s[0], cmd.fmt,
            cmd.args[0], cmd.args[1], cmd.args[2], cmd.args[3],
            cmd.args[4], cmd.args[5], cmd.args[6], cmd.args[7],
            cmd.args[8], cmd.args[9]);
      } else {
        *reinterpret_cast<string*>(cmd.args[0]) = s;
      }
      *cmd.available = true;
    } else {
      pthread_yield();
    }
  }
  pthread_exit(nullptr);
}

USBProxy::USBProxy() : host(), alive(true) {
  int i = pthread_create(&pid, nullptr, USBProxyRoutine, this);
  if (i) {
    perror("");
    throw exception();
  }
}

USBProxy::~USBProxy() {
  errno = 0;
  alive = false;
  timespec time = {0, 10000000};
  if (pthread_timedjoin_np(pid, nullptr, &time)) {
    pthread_cancel(pid);
  }
  if (errno) {
    perror("");
  }
}

void USBProxy::sendCmd(const string &s) {
  host.sendCmd(s);
}

void USBProxy::recvCmd(volatile bool *available, const char *fmt, ...) {
  *available = false;
  int argc = 0;
  char c;
  const char *ptr = fmt;
  while (c = *ptr++) {
    if (c == '%') {
      char next = *ptr++;
      if (next == '\0') {
        break;
      } else if (next != '%') {
        argc++;
      }
    }
  }

  std::vector<void*> args;
  va_list ap;
  va_start(ap, fmt);
  for (int i = 0; i < argc; i++) {
    args.push_back(va_arg(ap, void*));
  }
  va_end(ap);

  lock.lock();
  cmdQueue.push_back({available, fmt, args});
  lock.unlock();
}

void USBProxy::recvCmd(volatile bool *available, string *s) {
  *available = false;
  lock.lock();
  cmdQueue.push_back({available, nullptr, {s}});
  lock.unlock();
}

void USBProxy::recvCmdBlocked(string *s) {
  volatile bool ok = false;
  recvCmd(&ok, s);
  while (!ok) {
  };
}


void USBProxy::recvCmdBlocked(const char *fmt, ...) {
  volatile bool ok = false;
  int argc = 0;
  char c;
  const char *ptr = fmt;
  while (c = *ptr++) {
    if (c == '%') {
      char next = *ptr++;
      if (next == '\0') {
        break;
      } else if (next != '%') {
        argc++;
      }
    }
  }

  std::vector<void*> args;
  va_list ap;
  va_start(ap, fmt);
  for (int i = 0; i < argc; i++) {
    args.push_back(va_arg(ap, void*));
  }
  va_end(ap);

  lock.lock();
  cmdQueue.push_back({&ok, fmt, args});
  lock.unlock();
  while (!ok) {
  };
}

void* USBProxy2::USBProxyRoutine(void *arg) {
  USBProxy2 *self = reinterpret_cast<USBProxy2*>(arg);
  while (self->alive) {
    bool hasCmd = false;
    Command cmd;
    self->lock.lock();
    if (self->cmdQueue.size()) {
      hasCmd = true;
      cmd = self->cmdQueue.front();
      self->cmdQueue.pop_front();
    }
    self->lock.unlock();

    if (hasCmd) {
      self->host.sendCmd(cmd.cmd);
      if (cmd.writeBack) {
        self->host.recvCmd(*cmd.writeBack);
      } else {
        string temp;
        self->host.recvCmd(temp);
      }
      if (cmd.ok) {
        *cmd.ok = true;
      }
    } else {
      pthread_yield();
    }
  }
  pthread_exit(nullptr);
}

USBProxy2::USBProxy2() : host(), alive(true) {
  int i = pthread_create(&pid, nullptr, USBProxyRoutine, this);
  if (i) {
    perror("");
    throw exception();
  }
}

USBProxy2::~USBProxy2() {
  errno = 0;
  alive = false;
  timespec time = {0, 10000000};
  if (pthread_timedjoin_np(pid, nullptr, &time)) {
    pthread_cancel(pid);
  }
  if (errno) {
    perror("");
  }
}


void USBProxy2::sendCmd(const string &s, volatile bool *ok) {
  if (ok) {
    *ok = false;
  }
  lock.lock();
  cmdQueue.push_back({s, ok, nullptr});
  lock.unlock();
}

void USBProxy2::sendCmd(const string &s, volatile bool *ok, string &response) {
  *ok = false;
  lock.lock();
  cmdQueue.push_back({s, ok, &response});
  lock.unlock();
}

string USBProxy2::sendCmdBlocked(const string &s) {
  volatile bool ok = false;
  string reponse;
  sendCmd(s, &ok, reponse);
  while (!ok) ;
  return reponse;
}

USBProxy2 usbProxy;
