add_executable(${PROJECT_NAME}
src/main.cpp
src/SensorData.h
src/MQTTCallback.h
src/MQTTCallback.cpp
src/gRPCServer.h
src/gRPCServer.cpp
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
include_directories(${CMAKE_SOURCE_DIR}/protos)

target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(${CMAKE_PROJECT_NAME} pthread boost)