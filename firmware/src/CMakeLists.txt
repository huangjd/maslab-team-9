set(CMAKE_C_COMPILER "arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "arm-none-eabi-g++")
set(CMAKE_LINKER "arm-none-eabi-ld")

file(GLOB cpp "../teensy3/*.cpp" "*.cpp")
file(GLOB c "../teensy3/*.c" "*.c")
file(GLOB h "../teensy3/*.h" "../teensy3/*/*.h" "*.h")
set(CORE "${CMAKE_CURRENT_SOURCE_DIR}/../teensy3/")

include_directories(${CORE})

set(CMAKE_C_FLAGS "")
set(CMAKE_CXX_FLAGS "")
set(CMAKE_LINKER_FLAGS "")

add_definitions(-std=gnu++11 -mcpu=cortex-m4 -mthumb
                -felide-constructors -fno-exceptions -fno-rtti
                -DF_CPU=72000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH
                -D__MK20DX256__ -DARDUINO=16700 -DTEENSYDUINO=127
                -DUSING_MAKEFILE -I${CORE})

add_library(teensy STATIC ${cpp} ${c} ${h})

add_custom_command(OUTPUT co
                   COMMAND "${ARDUINO_ARM_TOOL}/arm-none-eabi-gcc"
                   ARGS "-Wall" "-g" "-Os"
                        "-mcpu=cortex-m4" "-mthumb" "-MMD"
                        "-DF_CPU=72000000" "-DUSB_SERIAL" "-DLAYOUT_US_ENGLISH"
                        "-D__MK20DX256__" "-DARDUINO=16700" "-DTEENSYDUINO=127"
                        "-DUSING_MAKEFILE"
                        "-I." "-I${CORE}"
                        "-c" ${c})
add_custom_target(build_teensy_c_files ALL DEPENDS co)

add_custom_command(OUTPUT cppo
                   COMMAND "${ARDUINO_ARM_TOOL}/arm-none-eabi-g++"
                   ARGS "-Wall" "-g" "-Os" "-std=gnu++11"
                        "-felide-constructors" "-fno-exceptions" "-fno-rtti"
                        "-mcpu=cortex-m4" "-mthumb" "-MMD"
                        "-DF_CPU=72000000" "-DUSB_SERIAL" "-DLAYOUT_US_ENGLISH"
                        "-D__MK20DX256__" "-DARDUINO=16700" "-DTEENSYDUINO=127"
                        "-DUSING_MAKEFILE"
                        "-I." "-I${CORE}"
                        "-c" ${cpp})
add_custom_target(build_teensy_cpp_files ALL DEPENDS cppo)

file(GLOB o "${CMAKE_CURRENT_BINARY_DIR}/*.o")
add_custom_command(OUTPUT elf
                   COMMAND "${ARDUINO_ARM_TOOL}/arm-none-eabi-gcc"
                   ARGS "-Os"
                        "-mcpu=cortex-m4" "-mthumb"
                        "-Wl,--gc-sections,--defsym=__rtc_localtime=0"
                        "--specs=nano.specs" "-T${CORE}/mk20dx256.ld" "-lm"
                        "-o" "main.elf" ${o}
                   COMMAND "${ARDUINO_ARM_TOOL}/arm-none-eabi-size"
                   ARGS "${CMAKE_CURRENT_BINARY_DIR}/main.elf"
                   COMMAND "${ARDUINO_ARM_TOOL}/arm-none-eabi-objcopy"
                   ARGS "-O" "ihex" "-R" ".eeprom" "${CMAKE_CURRENT_BINARY_DIR}/main.elf" "main.hex")
add_custom_target(build_teensy_main ALL DEPENDS elf)
return()
