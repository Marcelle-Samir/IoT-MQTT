#include "SensorsController.h"
#include <iostream>
#include <chrono>

SensorsController::SensorsController()
{
    createTemperatureSensor();
    createLightSensor();
}

SensorsController::~SensorsController()
{
    for (Sensor* sensor : sensors)
    {
        delete sensor;
    }
}

void SensorsController::createTemperatureSensor()
{
    sensors.push_back(new TemperatureSensor());
}

void SensorsController::createLightSensor()
{
    sensors.push_back(new LightSensor());
}

void SensorsController::initializeSensors()
{
    // To be implemented
}

void SensorsController::readAndProcessData()
{
    for (Sensor* sensor : sensors)
    {
        auto [reading, timestamp] = sensor->getSensorReading();
        storeValue(reading, "GenericSensor");

        std::cout << "Sensor Reading: " << reading << " at " << timestamp << std::endl;
        // to be implemented
        // send it to the MQTT server
    }
}

void SensorsController::storeValue(float reading, const std::string& sensorType)
{
    sensorValues.push_back({reading, std::chrono::system_clock::now().time_since_epoch().count()});
}
