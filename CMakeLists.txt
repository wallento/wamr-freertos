cmake_minimum_required(VERSION 3.1)

include(${CMAKE_CURRENT_LIST_DIR}/buildfiles/toolchain.cmake)

project(freertos-wamr)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# Set MCU
set(TARGET_MCU STM32F429)

if(TARGET_MCU STREQUAL STM32F401)
    set(linker_script ${CMAKE_CURRENT_LIST_DIR}/buildfiles/device_f401.ld)
elseif(${TARGET_MCU} STREQUAL STM32F429)
    set(linker_script ${CMAKE_CURRENT_LIST_DIR}/buildfiles/device_f429.ld)
else()
    message(FATAL_ERROR "Target MUC not set")
endif()

########################################
# Device specific interface library
# used by all targets being built
########################################
add_library(target_options INTERFACE)
target_compile_definitions(target_options INTERFACE
    -DSTM32F4=1
    -D${TARGET_MCU}
)
set(machine_flags -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)
target_compile_options(target_options INTERFACE
    ${machine_flags}
)

target_link_options(target_options INTERFACE
    -T ${linker_script}
    -g ${machine_flags} 
    -nostartfiles 
    -fno-common
)

######################
# Third party stuff
######################
add_subdirectory(third_party/libopencm3)
add_subdirectory(third_party/freeRTOS)
add_subdirectory(third_party/wasm-micro-runtime)

#####################
# Executable
#####################
add_executable(freertos-wamr)
target_sources(freertos-wamr 
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/init.c
)
target_include_directories(freertos-wamr 
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)
target_link_libraries(freertos-wamr PUBLIC 
    PUBLIC libopencm3_stm32_f4
    PUBLIC freeRTOS
    PUBLIC iwasm_static
)
