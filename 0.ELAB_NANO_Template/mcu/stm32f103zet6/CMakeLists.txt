set(LIB_NAME "stm32f1xx_libaray")


set( STM32_HAL_SRC
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_usart.c
STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c
)

set(STM_SYS_CORE_SRC
CMSIS/Device/ST/STM32F1xx/Source/Templates/system_stm32f1xx.c
CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/startup_stm32f103xe.s
)

add_library(${LIB_NAME} STATIC "")

#将库文件输出到构建目录的Lib子目录
set_target_properties(${LIB_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

target_sources(${LIB_NAME} PUBLIC
    ${STM32_HAL_SRC}
    ${STM_SYS_CORE_SRC}
)

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/STM32F1xx_HAL_Driver/Inc
    ${CMAKE_CURRENT_LIST_DIR}/CMSIS/Device/ST/STM32F1xx/Include
    ${CMAKE_CURRENT_LIST_DIR}/CMSIS/Include
)


if(__CHIP_TYPE__ STREQUAL "STM32F103ZET6")
    target_compile_definitions(${LIB_NAME} PUBLIC STM32F103xE 
    USE_HAL_DRIVER
    STM32F103xE
    )

    target_link_options(${LIB_NAME} PUBLIC
        -T "${CMAKE_CURRENT_LIST_DIR}/CMSIS/Device/ST/STM32F1xx/Source/Templates/gcc/linker/STM32F103XE_FLASH.ld"
    )

endif()