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

double LightSensor::getSensorReading()
{
    double lightReading = 5.7;
    std::cout << "Reading data from Light Sensor." << std::endl;
    return lightReading;
}

void LightSensor::run(mqtt::async_client& client)
{
    while (true)
    {
        double lightIntensity = getSensorReading();
        Sensor::publishReading(client, "sensors/light", lightIntensity);
        std::this_thread::sleep_for(std::chrono::seconds(7));
    }
}