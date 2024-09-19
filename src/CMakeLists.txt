add_executable(${PROJECT_NAME}
src/main.cpp
src/Sensor.h
src/Sensor.cpp
src/SensorsController.h
src/SensorsController.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(${CMAKE_PROJECT_NAME} pthread boost)