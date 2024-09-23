#include "LightSensor.h"
#include <iostream>

LightSensor::~LightSensor()
{
}

double LightSensor::getSensorReading()
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    double lightReading = generateReading();
    std::cout << "Reading data from Light Sensor." << std::endl;
    return lightReading;
}

void LightSensor::run(mqtt::async_client& client)
{
    std::cout << __FUNCTION__ << " is Called in LightSensor." << std::endl;
    while (true)
    {
        double lightIntensity = getSensorReading();
        Sensor::publishReading(client, "sensors/" + sensorId, lightIntensity);
        std::cout << "new light sensor Reading data has been published." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(7));
    }
}

double LightSensor::generateReading()
{
    static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(0.0, 1000.0);
    return distribution(generator);
}

void LightSensor::storeValue(double reading)
{
    std::cout << __FUNCTION__ << "is Called for a light sensor." << std::endl;
    std::lock_guard<std::mutex> lock(sensorMutex);
    if (sensorValues.size() >= maxSensorValuesSize)
    {
        sensorValues.pop_front();
    }
    sensorValues.push_back({sensorId, reading});
    std::cout << "Stored reading from " << sensorId << ": " << reading << std::endl;
}

std::string LightSensor::getSensorData()
{
    std::cout << __FUNCTION__ << " is called in lightSensor." << std::endl;
    std::lock_guard<std::mutex> lock(sensorMutex);
    std::string data;
    for (const auto& reading : sensorValues)
    {
        data += reading.first + ": " + std::to_string(reading.second) + "\n";
    }
    return data;
}

std::string LightSensor::getSpecificSensorData(const std::string& requestedSensorId)
{
    std::cout << __FUNCTION__ << " " << requestedSensorId << " light sensor data" << std::endl;

    std::lock_guard<std::mutex> lock(sensorMutex);
    std::string data;
    for (const auto& reading : sensorValues)
    {
        std::cout << __FUNCTION__ << " " << reading.first << " " << requestedSensorId << " light sensor data" << std::endl;
        if (reading.first == requestedSensorId)
        {
            data += reading.first + ": " + std::to_string(reading.second) + "\n";
        }
    }
    return data;
}
