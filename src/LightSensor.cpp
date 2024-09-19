#include "LightSensor.h"
#include <iostream>

LightSensor::LightSensor()
{
}

LightSensor::~LightSensor()
{
}

void LightSensor::reportSensorReading(float reading)
{
    std::cout << "Light Sensor initialized." << std::endl;
}

std::pair<float, long long> LightSensor::getSensorReading()
{
    std::pair<float, long long> lightReading;
    std::cout << "Reading data from Light Sensor." << std::endl;
    return lightReading;
}

