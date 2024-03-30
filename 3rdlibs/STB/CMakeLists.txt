set (TARGET_NAME STB)

add_library (${TARGET_NAME} OBJECT stb_image.cpp)
target_include_directories(${TARGET_NAME} PUBLIC ./)