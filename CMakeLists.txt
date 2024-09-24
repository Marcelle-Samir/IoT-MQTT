cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/conan")
include(${CMAKE_TOOLCHAIN_FILE})

project(IoT-MQTT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
  src/main.cpp
  src/SensorData.h
  src/MQTTCallback.h
  src/MQTTCallback.cpp
  src/RestApi.h
  src/RestApi.cpp
  src/Sensor.h
  src/Sensor.cpp
  src/TemperatureSensor.h
  src/TemperatureSensor.cpp
  src/LightSensor.h
  src/LightSensor.cpp
  src/SensorsController.h
  src/SensorsController.cpp
)

add_executable(IoT-MQTT ${SOURCES})

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(eclipse-paho-mqtt-c REQUIRED)
find_package(PahoMqttCpp REQUIRED)
find_package(Pistache REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

target_link_libraries(IoT-MQTT
    nlohmann_json::nlohmann_json
    CURL::libcurl
    eclipse-paho-mqtt-c::paho-mqtt3as-static
    PahoMqttCpp::paho-mqttpp3-static
    Pistache::Pistache
    pthread
)

target_include_directories(IoT-MQTT PRIVATE ${CONAN_INCLUDE_DIRS})

enable_testing()
add_subdirectory(test)
