# Install script for directory: /home/pi/Friendly-LwM2M-Client-1/wpp/registry/objects

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/m_0_lwm2m_security/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/m_1_lwm2m_server/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/m_3_device/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_2_lwm2m_access_control/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3201_digital_output/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3202_analog_input/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3203_analog_output/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3300_generic_sensor/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3303_temperature/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3311_light_control/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3316_voltage/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3317_current/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3322_load/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3323_pressure/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3328_power/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3336_ipso3336/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_3347_push_button/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_4_connectivity_monitoring/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_5_firmware_update/cmake_install.cmake")
  include("/home/pi/Friendly-LwM2M-Client-1/build/wpp/wpp/registry/objects/o_6_location/cmake_install.cmake")

endif()

