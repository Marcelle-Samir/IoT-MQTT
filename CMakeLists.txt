cmake_minimum_required(VERSION 3.12)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_BINARY_DIR}/conan/conan_toolchain.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

project(IoT-MQTT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(nlohmann_json REQUIRED)
find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(eclipse-paho-mqtt-c REQUIRED)
find_package(PahoMqttCpp REQUIRED)
find_package(Pistache REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(absl REQUIRED)
find_package(c-ares REQUIRED)
find_package(re2 REQUIRED)
find_package(Protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTOBUF_GENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/protos")
file(MAKE_DIRECTORY ${PROTOBUF_GENERATED_DIR})

include_directories(${PROTOBUF_GENERATED_DIR})

set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/protos/sensor.proto)

add_custom_command(
    OUTPUT "${PROTOBUF_GENERATED_DIR}/sensor.pb.cc" "${PROTOBUF_GENERATED_DIR}/sensor.pb.h"
           "${PROTOBUF_GENERATED_DIR}/sensor.grpc.pb.cc" "${PROTOBUF_GENERATED_DIR}/sensor.grpc.pb.h"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${PROTOBUF_GENERATED_DIR}" --cpp_out "${PROTOBUF_GENERATED_DIR}"
         --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
         -I "${CMAKE_CURRENT_SOURCE_DIR}/protos" "${PROTO_FILES}"
    DEPENDS "${PROTO_FILES}"
    COMMENT "Generating protobuf and gRPC files from sensor.proto"
)

set(PROTO_SRCS "${PROTOBUF_GENERATED_DIR}/sensor.pb.cc")
set(PROTO_HDRS "${PROTOBUF_GENERATED_DIR}/sensor.pb.h")
set(GRPC_SRCS "${PROTOBUF_GENERATED_DIR}/sensor.grpc.pb.cc")
set(GRPC_HDRS "${PROTOBUF_GENERATED_DIR}/sensor.grpc.pb.h")

set(SOURCES
    src/main.cpp
    src/SensorData.h
    src/MQTTCallback.h
    src/MQTTCallback.cpp
    src/gRPCServer.h
    src/gRPCServer.cpp
    src/gRPCClient.h
    src/gRPCClient.cpp
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
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

add_executable(IoT-MQTT ${SOURCES})

target_link_libraries(IoT-MQTT
    nlohmann_json::nlohmann_json
    CURL::libcurl
    eclipse-paho-mqtt-c::paho-mqtt3as-static
    PahoMqttCpp::paho-mqttpp3-static
    Pistache::Pistache
    abseil::abseil
    c-ares::cares
    re2::re2
    protobuf::libprotobuf
    gRPC::grpc++
    pthread
)

target_include_directories(IoT-MQTT PRIVATE ${CONAN_INCLUDE_DIRS})

enable_testing()
add_subdirectory(test)
