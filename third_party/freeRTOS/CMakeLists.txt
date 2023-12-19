

set(FREERTOS_PORT GCC_ARM_CM4F CACHE STRING "" FORCE)
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)

# freertos_config is linked by freertos_kernel_include which is linked by freertos_kernel
add_library(freertos_config INTERFACE)
target_include_directories(freertos_config SYSTEM INTERFACE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/include
    ${CMAKE_CURRENT_LIST_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/
)
target_link_libraries(freertos_config INTERFACE
    target_options
)

add_subdirectory(FreeRTOS-Kernel)


add_library(freeRTOS INTERFACE)
target_link_libraries(freeRTOS INTERFACE 
    freertos_config
    freertos_kernel
    freertos_kernel_port)
