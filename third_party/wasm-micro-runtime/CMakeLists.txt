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
target_link_libraries(iwasm_static 
    PUBLIC target_options
    PUBLIC m
)

target_include_directories(iwasm_static INTERFACE ${WAMR_ROOT_DIR}/core/iwasm/include)
target_include_directories(iwasm_static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../freeRTOS/FreeRTOS-Kernel/include)
target_include_directories(iwasm_static PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(iwasm_static PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../freeRTOS/FreeRTOS-Kernel/portable/GCC/ARM_CM4F/)
install (TARGETS iwasm_static ARCHIVE DESTINATION lib)
