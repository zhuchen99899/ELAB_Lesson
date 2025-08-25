set(LIB_NAME "elab_libaray")

set(ELAB_SRC
    common/elab_log.c
    common/elab_common.c
    common/elab_assert.c
    3rd/list/list_iterator.c
    3rd/list/list_node.c
    3rd/list/list.c
    3rd/lwrb/lwrb.c
    3rd/ringbuf/ringbuf.c
    3rd/state_machine/state_machine.c
    elib/elib_crc.c
    elib/elib_queue.c
    elib/hash_table.c
)


set(XFUSION_SRC
    3rd/xfusion/xf_utils_log/xf_utils_log_dump.c
)

add_library(${LIB_NAME} STATIC "")

#将库文件输出到构建目录的Lib子目录
set_target_properties(${LIB_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)
target_sources(${LIB_NAME} PUBLIC
    ${ELAB_SRC}
    ${XFUSION_SRC}
)


target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/common
    ${CMAKE_CURRENT_LIST_DIR}/3rd/xfusion/xf_utils_log
    ${CMAKE_CURRENT_LIST_DIR}/3rd/xfusion/xf_common
    ${CMAKE_CURRENT_LIST_DIR}/3rd/xfusion
    ${CMAKE_CURRENT_LIST_DIR}/3rd/lwrb
    ${CMAKE_CURRENT_LIST_DIR}/3rd/list
    ${CMAKE_CURRENT_LIST_DIR}/3rd/ringbuf
    ${CMAKE_CURRENT_LIST_DIR}/3rd/state_machine
    ${CMAKE_CURRENT_LIST_DIR}/elib

)