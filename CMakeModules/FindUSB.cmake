find_path(USB_INCLUDE_DIR usb.h)
find_library(USB_LIBRARIES NAMES usb)

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(USB DEFAULT_MSG
                                  USB_LIBRARIES USB_INCLUDE_DIR)

mark_as_advanced(USB_INCLUDE_DIR USB_LIBRARIES)

