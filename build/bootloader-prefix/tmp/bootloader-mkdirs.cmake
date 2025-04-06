# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/mujtaba/esp/v5.4/esp-idf/components/bootloader/subproject"
  "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader"
  "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix"
  "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix/tmp"
  "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix/src/bootloader-stamp"
  "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix/src"
  "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mujtaba/Documents/Embedded/Project/esp-rainmaker/examples/temperature_sensor_a/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
