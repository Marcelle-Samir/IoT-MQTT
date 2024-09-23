#include "TemperatureSensor.h"
#include <iostream>

TemperatureSensor::TemperatureSensor()
{
}

TemperatureSensor::~TemperatureSensor()
{
}

double TemperatureSensor::getSensorReading()
{
    double temperatureReading = generateReading();
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

double TemperatureSensor::generateReading()
{
    static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(20.0, 30.0);
    return distribution(generator);
}