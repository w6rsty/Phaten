aux_source_directory(./ SRC_LIST)
aux_source_directory(./Core SRC_LIST)
aux_source_directory(./Graphics SRC_LIST)
aux_source_directory(./Input SRC_LIST)

add_executable(main ${SRC_LIST})

target_include_directories(main PRIVATE ./)

set(PT_LIBS
    debugbreak
    glad
)

target_link_libraries(main ${PT_LIBS})