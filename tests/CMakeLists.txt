add_executable(test_is31fl3731 test_is31fl3731.c)
target_sources(test_is31fl3731 PRIVATE ../src/is31fl3731/is31fl3731.c)
target_include_directories(test_is31fl3731 PRIVATE ../src/is31fl3731)
target_link_libraries(test_is31fl3731 Unity)

add_executable(test_sh1106 test_sh1106.c)
target_sources(test_sh1106 PRIVATE ../src/sh1106/sh1106.c)
target_include_directories(test_sh1106 PRIVATE ../src/sh1106 ../src)
target_link_libraries(test_sh1106 Unity)

add_test(NAME test_is31fl3731 COMMAND test_is31fl3731)
add_test(NAME test_sh1106 COMMAND test_sh1106)

