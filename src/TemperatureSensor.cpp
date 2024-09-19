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

std::pair<float, long long> TemperatureSensor::getSensorReading()
{
    std::pair<float, long long> temperatureReading;
    std::cout << "Reading data from Temperature Sensor." << std::endl;
    return temperatureReading;
}

