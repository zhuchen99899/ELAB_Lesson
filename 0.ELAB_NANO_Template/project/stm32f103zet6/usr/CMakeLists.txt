set(__PROJ_NAME__ "Template" CACHE STRING "Proj name")
set(__CHIP_TYPE__ "STM32F103ZET6" CACHE STRING "Chip type")
set(__TOOLCHAIN__ "ARMGCC" CACHE STRING "Toolchain type")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/output)
set(HEX_FILE ${EXECUTABLE_OUTPUT_PATH}/${__PROJ_NAME__}.hex)
set(BIN_FILE ${EXECUTABLE_OUTPUT_PATH}/${__PROJ_NAME__}.bin)
set(MAP_FILE ${EXECUTABLE_OUTPUT_PATH}/${__PROJ_NAME__}.map)

cmake_minimum_required(VERSION 3.20)

#设置交叉编译工具配置文件
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../ide/gcc/scripts/cmake/armgcc_cm3.cmake")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${__PROJ_NAME__} C CXX ASM)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()


# -----------------------------------------------------------------------------

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../../../mcu/stm32f103zet6 ${CMAKE_CURRENT_BINARY_DIR}/stm32f1xx_libaray)

set(PRO_SRC
    main.c
    core/stm32f1xx_it.c
    core/stm32f1xx_hal_msp.c
    bsp/bsp.c
)

add_executable(${__PROJ_NAME__})
target_sources(${__PROJ_NAME__} PRIVATE ${PRO_SRC})

target_include_directories(${__PROJ_NAME__} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/core
    ${CMAKE_CURRENT_LIST_DIR}/bsp
)


target_link_libraries(${__PROJ_NAME__} PRIVATE "stm32f1xx_libaray")

#生成map文件
target_link_options(${__PROJ_NAME__} PRIVATE
    -Wl,-Map=${MAP_FILE}
)

#生成elf文件
set_target_properties(
    ${__PROJ_NAME__} PROPERTIES
    OUTPUT_NAME "${__PROJ_NAME__}.elf"
)

#elf转化成hex,bin文件
add_custom_command(TARGET ${__PROJ_NAME__} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${__PROJ_NAME__}> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${__PROJ_NAME__}> ${BIN_FILE}
    COMMENT "Convert ELF to HEX and BIN files"
)