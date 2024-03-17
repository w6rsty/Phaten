set(TARGET_NAME Phaten)

add_engine_directory(Core)
add_engine_directory(Graphics)
add_engine_directory(Input)
add_engine_directory(Math)

set(PT_LIBS
    glad
    SDL2-static
)

add_executable(${TARGET_NAME} ${SOURCE_FILES} main.cpp)

target_include_directories(${TARGET_NAME} PUBLIC .)

target_link_libraries(${TARGET_NAME} PRIVATE ${PT_LIBS})