# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/pi/Friendly-LwM2M-Client/2305-Wakaama/examples/shared/tinydtls"
  "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/src/external_tinydtls-build"
  "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix"
  "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/tmp"
  "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/src/external_tinydtls-stamp"
  "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/src"
  "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/src/external_tinydtls-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/src/external_tinydtls-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/pi/Friendly-LwM2M-Client/build/examples/linux/external_tinydtls-prefix/src/external_tinydtls-stamp${cfgdir}") # cfgdir has leading slash
endif()
