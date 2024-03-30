set(TARGET_NAME Phaten)

add_engine_directory(Core)
add_engine_directory(Graphics)
add_engine_directory(Input)
add_engine_directory(IO)
add_engine_directory(Math)
add_engine_directory(Object)
add_engine_directory(Renderer)
add_engine_directory(Resource)
add_engine_directory(Scene)
add_engine_directory(Thread)

set(PT_LIBS
    glad
    SDL2-static
    imgui
    STB
)

if (WIN32)
    list(APPEND PT_LIBS SDL2main)
endif()

add_library(${TARGET_NAME} STATIC ${SOURCE_FILES})

target_include_directories(${TARGET_NAME} PUBLIC .)

target_link_libraries(${TARGET_NAME} PUBLIC ${PT_LIBS})

target_compile_features(Phaten PUBLIC cxx_std_17)