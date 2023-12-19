
#################################
# libopencm3_base
#################################
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_LIST_DIR}/libopencm3/include/libopencm3/stm32/f4/nvic.h
  COMMAND ${CMAKE_CURRENT_LIST_DIR}/libopencm3/scripts/irq2nvic_h ./include/libopencm3/stm32/f4/irq.json
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/libopencm3
  VERBATIM
)
add_library(libopencm3_base
    ${CMAKE_CURRENT_LIST_DIR}/libopencm3/include/libopencm3/stm32/f4/nvic.h
)
target_include_directories(libopencm3_base
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/include
)
target_link_libraries(libopencm3_base
    PUBLIC target_options
)
target_sources(libopencm3_base
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/assert.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/dwt.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/nvic.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/scb.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/sync.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/systick.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/cm3/vector.c
)

#################################
# libopencm3_stm32_common
#################################
add_library(libopencm3_stm32_common)
target_link_libraries(libopencm3_stm32_common
    PUBLIC libopencm3_base
)
target_sources(libopencm3_stm32_common
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/adc_common_v1.c 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/adc_common_v1_multi.c 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/adc_common_f47.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/crc_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/crypto_common_f24.c 
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/dac_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/dac_common_v1.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/dcmi_common_f47.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/desig_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/desig_common_v1.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/dma_common_f24.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/dma2d_common_f47.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/dsi_common_f47.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/exti_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/flash_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/flash_common_f.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/flash_common_f24.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/flash_common_idcache.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/fmc_common_f47.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/gpio_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/gpio_common_f0234.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/hash_common_f24.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/i2c_common_v1.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/iwdg_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/lptimer_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/ltdc_common_f47.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/pwr_common_v1.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/rcc_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/rng_common_v1.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/rtc_common_l1f024.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/spi_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/spi_common_v1.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/spi_common_v1_frf.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/timer_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/timer_common_f0234.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/timer_common_f24.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/usart_common_all.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/usart_common_f124.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/common/quadspi_common_v1.c
)

###########################
# libopencm3_stm32_f4
###########################
add_library(libopencm3_stm32_f4)
target_link_libraries(libopencm3_stm32_f4
    PUBLIC libopencm3_stm32_common
)
target_sources(libopencm3_stm32_f4
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/crypto.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/flash.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/i2c.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/Makefile
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/pwr.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/rcc.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/rng.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/stm32/f4/rtc.c
)

#############################
# libopencm3_usb
#############################
add_library(libopencm3_usb)
target_sources(libopencm3_usb
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_standard.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_control.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_msc.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_hid.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_audio.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_cdc.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_midi.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_dwc_common.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_f107.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/usb/usb_f207.c
)
target_link_libraries(libopencm3_usb
    PUBLIC libopencm3_base
)

###########################
# libopencm3_ethernet
###########################
add_library(libopencm3_ethernet)
target_sources(libopencm3_ethernet
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/ethernet/mac.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/ethernet/phy.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/ethernet/mac_stm32fxx7.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/libopencm3/lib/ethernet/phy_ksz80x1.c
)
target_link_libraries(libopencm3_ethernet
    PUBLIC libopencm3_base
)