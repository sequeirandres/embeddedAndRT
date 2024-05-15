cmake_minimum_required(VERSION 3.12)

set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)
include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/external/pico_sdk_import.cmake)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(main_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_library(freertos STATIC
  FreeRTOS-Kernel
  FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/port.c
)

target_include_directories(freertos PUBLIC
  .
  FreeRTOS-Kernel/include
  FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/include
)

target_link_libraries(freertos
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap3
  pico_stdlib
)

add_executable(
  blink
  blink.c
)

pico_enable_stdio_usb(blink 1)

pico_add_extra_outputs(blink)
target_link_libraries(blink pico_stdlib freertos)
