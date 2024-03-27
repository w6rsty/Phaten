add_executable(PhatenTest main.cpp)

set(TEST_LIBS
    Phaten
)

target_link_libraries(PhatenTest PRIVATE ${TEST_LIBS})
