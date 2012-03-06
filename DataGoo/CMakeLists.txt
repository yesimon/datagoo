#====================================================================#
#  Settings                                                          #
#====================================================================#
link_directories(~/Documents/Arduino/libraries)

set(FIRMWARE_NAME datagoo)

set(${FIRMWARE_NAME}_BOARD uno)               # Arduino Target board
set(${FIRMWARE_NAME}_SRCS DataGoo.cpp LedDisplay.cpp)  # Firmware sources
set(${FIRMWARE_NAME}_HDRS LedDisplay.h)

set(${FIRMWARE_NAME}_PORT /dev/ttyACM0)            # Serial upload port


#====================================================================#
#  Target generation                                                 #
#====================================================================#
generate_arduino_firmware(${FIRMWARE_NAME})
