set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-O2 -Wall -s")
add_definitions("-DUSE_LIBUSB")

add_executable(teensy_loader teensy_loader_cli.c)

find_library(LIBUSB usb)
if(LIBUSB-NOTFOUND)
  message(FATAL_ERROR "libusb Not Found!")
endif(LIBUSB-NOTFOUND)
target_link_libraries(teensy_loader ${LIBUSB})
