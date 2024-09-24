#include "TemperatureSensor.h"
#include <iostream>

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
    std::cout << __FUNCTION__ << " is Called in TemperatureSensor." << std::endl;
    while (true)
    {
        double temperature = getSensorReading();
        Sensor::publishReading(client, "sensors/" + sensorId, temperature);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}

double TemperatureSensor::generateReading()
{
    static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> distribution(20.0, 30.0);
    return distribution(generator);
}

void TemperatureSensor::storeValue(double reading)
{
    std::cout << __FUNCTION__ << " is Called for a temperature sensor." << std::endl;
    std::lock_guard<std::mutex> lock(sensorMutex);
    if (sensorValues.size() >= maxSensorValuesSize)
    {
        sensorValues.pop_front();
    }
    sensorValues.push_back({sensorId, reading});
    std::cout << "Stored reading from " << sensorId << ": " << reading << std::endl;
}

std::string TemperatureSensor::getSensorData()
{
    std::lock_guard<std::mutex> lock(sensorMutex);
    std::string data;
    for (const auto& reading : sensorValues)
    {
        data += reading.first + ": " + std::to_string(reading.second) + "\n";
    }
    return data;
}

std::string TemperatureSensor::getSpecificSensorData(const std::string& requestedSensorId)
{
    std::lock_guard<std::mutex> lock(sensorMutex);
    std::string data;
    for (const auto& reading : sensorValues)
    {
        if (reading.first == requestedSensorId)
        {
            data += reading.first + ": " + std::to_string(reading.second) + "\n";
        }
    }
    return data;
}

SensorData TemperatureSensor::calculateSensorData()
{
    SensorData m_sensorData;
    double sum = 0.0;
    double minReading = std::numeric_limits<double>::max();
    double maxReading = std::numeric_limits<double>::lowest();
    for (const auto& reading : sensorValues) {
        double value = reading.second;
        sum += value;
        if (value < minReading) {
            minReading = value;
        }
        if (value > maxReading) {
            maxReading = value;
        }
    }
    m_sensorData.averageReading = sensorValues.empty() ? 0.0 : sum / sensorValues.size();
    m_sensorData.minReading = sensorValues.empty() ? 0.0 : minReading;
    m_sensorData.maxReading = sensorValues.empty() ? 0.0 : maxReading;
    return m_sensorData;
}