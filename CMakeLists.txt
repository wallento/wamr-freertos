cmake_minimum_required(VERSION 3.1)

include(buildfiles/toolchain.cmake)
set (machine_flags -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16)

project(freertos-wamr)

set (CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# libopenmc3 dependency
set(OPENCM3DIR ${CMAKE_CURRENT_LIST_DIR}/libopencm3)
set(OPENCM3LIB ${OPENCM3DIR}/lib/libopencm3_stm32f4.a)
add_custom_command(OUTPUT ${OPENCM3LIB}
                    COMMAND make TARGETS='stm32/f4 CC=arm-none-eabi-gcc'
                    WORKING_DIRECTORY ${OPENCM3DIR})

# FreeRTOS dependency
set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "" FORCE)
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include) # The config file directory
add_subdirectory(FreeRTOS-Kernel)

target_compile_options(freertos_kernel PRIVATE ${machine_flags})
target_compile_options(freertos_kernel_port PRIVATE ${machine_flags})

# wasm-micro-runtime
set (WAMR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/wasm-micro-runtime)
set (WAMR_BUILD_PLATFORM "freertos")

set (WAMR_BUILD_TARGET "THUMB")

set (WAMR_BUILD_INTERP 1)
set (WAMR_BUILD_AOT 0)
set (WAMR_BUILD_LIBC_BUILTIN 1)
set (WAMR_BUILD_LIBC_WASI 0)

include (${WAMR_ROOT_DIR}/build-scripts/runtime_lib.cmake)

add_library(iwasm_static STATIC ${WAMR_RUNTIME_LIB_SOURCE})
set_target_properties (iwasm_static PROPERTIES OUTPUT_NAME vmlib)
target_compile_options(iwasm_static PUBLIC ${machine_flags})
target_include_directories(iwasm_static INTERFACE ${WAMR_ROOT_DIR}/core/iwasm/include)
target_include_directories(iwasm_static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include)
target_include_directories(iwasm_static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(iwasm_static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/)
install (TARGETS iwasm_static ARCHIVE DESTINATION lib)

# Executable
add_executable(freertos-wamr ${CMAKE_CURRENT_LIST_DIR}/src/main.c)

target_include_directories(freertos-wamr PUBLIC ${OPENCM3DIR}/include)
target_include_directories(freertos-wamr PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(freertos-wamr PRIVATE ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include)
target_include_directories(freertos-wamr PRIVATE ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/)
target_compile_options(freertos-wamr PUBLIC ${machine_flags})
target_compile_options(freertos-wamr PUBLIC -DSTM32F4=1)

target_sources(freertos-wamr PRIVATE src/init.c ${OPENCM3LIB})

target_link_libraries(freertos-wamr PUBLIC ${OPENCM3LIB})
target_link_libraries(freertos-wamr PUBLIC freertos_kernel freertos_config freertos_kernel_port)
target_link_libraries(freertos-wamr PUBLIC iwasm_static)
target_link_libraries(freertos-wamr PUBLIC m)
target_link_options(freertos-wamr PUBLIC -g ${machine_flags} -nostartfiles -fno-common)
target_link_options(freertos-wamr PUBLIC -T ${CMAKE_CURRENT_LIST_DIR}/buildfiles/device.ld)
