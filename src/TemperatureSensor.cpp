#include "TemperatureSensor.h"
#include <iostream>

TemperatureSensor::TemperatureSensor()
{
}

TemperatureSensor::~TemperatureSensor()
{
}

void TemperatureSensor::reportSensorReading(float reading)
{
    std::cout << "Temperature Sensor initialized." << std::endl;
}

double TemperatureSensor::getSensorReading()
{
    double temperatureReading = 33.21;
    std::cout << "Reading data from Temperature Sensor." << std::endl;
    return temperatureReading;
}

void TemperatureSensor::run(mqtt::async_client& client)
{
    while (true)
    {
        double temperature = getSensorReading();
        Sensor::publishReading(client, "sensors/temperature", temperature);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}