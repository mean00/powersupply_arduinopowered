#=============================================================================#
# cmake arduino cloned from https://github.com/ncasaril/arduino-cmake.git     #
#=============================================================================#
set(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake) # Arduino Toolchain


cmake_minimum_required(VERSION 2.8)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(ArduinoExample C CXX)
#
set(ARDUINO_DEFAULT_BOARD nano)        # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) # Default Port, when not specified
set(ARDUINO_CPU          atmega328)

# this is needed for the nano, it should be set automatically, maybe something changed with sdk 1.8.x ?
set(ARDUINO_CPUMENU .menu.cpu.${ARDUINO_CPU})
#
print_board_list()
print_programmer_list()

PRINT_SETTINGS(${ARDUINO_DEFAULT_BOARD})

# Where you store your downloaded libs or manually installed
SET(CUSTOM_LIB_DIR /home/fx/Arduino/libraries)

link_directories( ${CUSTOM_LIB_DIR})

# u8glib is complicated and needs special care
include_directories(${CUSTOM_LIB_DIR}/U8g2/src)
set(U8g2lib_RECURSE True)
set(U8g2_RECURSE True)
#
# Be careful with the includes in the ino files
# #include <foo.h> // this is a comment
# will fail
# same with extra tabs or space after the #include "blah.h"
#
SET(libPrefix ${ARDUINO_DEFAULT_BOARD}.${ARDUINO_CPU}_)

# This reduces size by 10% or so
#ADD_DEFINITIONS("-ffunction-sections -fdata-sections ")
generate_arduino_firmware(power_supply 
                SKETCH powersupply_arduinopowered.ino
                SRCS  runLoop.cpp power_screen.cpp wav_irotary.cpp pow_rotaryCurrentControl.cpp
                LIBS ${libPrefix}simpler_INA219 
                LIBS ${libPrefix}Wire 
                LIBS ${libPrefix}U8g2lib 
                PORT ${ARDUINO_DEFAULT_PORT}
                BOARD_CPU ${ARDUINO_CPU}
                )

# EOF
