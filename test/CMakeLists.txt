include(GoogleTest)
include(${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(SOURCES
    Iot-MQTT_test.cpp
)

add_executable(IOTMQTT_test ${SOURCES})
find_package(GTest REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

target_link_libraries(IOTMQTT_test
    GTest::gtest
    GTest::gtest_main
    nlohmann_json::nlohmann_json
    CURL::libcurl
    ${UUID_LIBRARIES}
    pthread)

gtest_discover_tests(IOTMQTT_test)

add_test(NAME IOTMQTT_test COMMAND IOTMQTT_test)