cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/conan")
include(${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake)

project(IoT-MQTT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
  src/main.cpp
)

add_executable(IoT-MQTT ${SOURCES})

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

target_link_libraries(IoT-MQTT
    nlohmann_json::nlohmann_json
    CURL::libcurl
    ${UUID_LIBRARIES}
    pthread
)

target_include_directories(IoT-MQTT PRIVATE ${CONAN_INCLUDE_DIRS})

enable_testing()
add_subdirectory(test)