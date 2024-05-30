cmake_minimum_required(VERSION 3.12)

set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel)
include(pico-sdk/pico_sdk_init.cmake)
include(pico-sdk/external/pico_sdk_import.cmake)
include(FreeRTOS-Kernel/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(main_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico_w) # pico or pico_w

if (DEFINED ENV{WIFI_SSID} AND (NOT WIFI_SSID))
  set(WIFI_SSID $ENV{WIFI_SSID})
  message("Using WIFI_SSID from environment ('${WIFI_SSID}')")
else()
  message(FATAL_ERROR "WIFI_SSID not set on ENV")
endif()

if (DEFINED ENV{WIFI_PASSWORD} AND (NOT WIFI_PASSWORD))
  set(WIFI_PASSWORD $ENV{WIFI_PASSWORD})
  message("Using WIFI_PASSWORD from environment")
else()
  message(FATAL_ERROR "WIFI_PASSWORD not set on ENV")
endif()

if (DEFINED ENV{SERVER_PORT})
  set(SERVER_PORT $ENV{SERVER_PORT})
  message("Using SERVER_PORT from environment")
else()
  message("Using default SERVER_PORT")
  set(SERVER_PORT 5000)
endif()

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
  FreeRTOS-Kernel-Heap4
  pico_stdlib
)

add_executable(blink
  blink.c
)

add_executable(socket
  socket.c
)

target_include_directories(socket PRIVATE
    ${PICO_LWIP_CONTRIB_PATH}/apps/ping
    ${TOP}/lib/lwip/src/include/
)
target_compile_definitions(socket PRIVATE
  WIFI_SSID=\"${WIFI_SSID}\"
  WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
  SERVER_PORT=${SERVER_PORT}
  NO_SYS=0 
)

pico_enable_stdio_usb(blink 1)
pico_add_extra_outputs(blink)
target_link_libraries(blink pico_stdlib freertos)

pico_enable_stdio_usb(socket 1)
pico_add_extra_outputs(socket 1)
target_link_libraries(socket pico_stdlib freertos 
  pico_cyw43_arch_lwip_sys_freertos
)

