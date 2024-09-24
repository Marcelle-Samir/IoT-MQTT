#include "Sensor.h"
#include <iostream>

Sensor::~Sensor()
{
}

void Sensor::publishReading(mqtt::async_client& client, const std::string& topic, double value)
{
    std::cout << __FUNCTION__ << " is Called." << std::endl;
    mqtt::message_ptr pubmsg = mqtt::make_message(topic, std::to_string(value));
    pubmsg->set_qos(1);
    client.publish(pubmsg);
}

std::string Sensor::getSpecificSensorData(const std::string& requestedSensorId)
{
    std::cout << __FUNCTION__ << " " << requestedSensorId << " light sensor data" << std::endl;

    std::lock_guard<std::mutex> lock(sensorMutex);
    std::string data;

    for (auto it = sensorValues.rbegin(); it != sensorValues.rend(); ++it)
    {
        if (it->first == requestedSensorId)
        {
            data = it->first + ": " + std::to_string(it->second) + "\n";
            break;
        }
    }

    return data;
}

SensorData Sensor::calculateSensorData()
{
    SensorData m_sensorData;
    double sum = 0.0;
    double minReading = std::numeric_limits<double>::max();
    double maxReading = std::numeric_limits<double>::lowest();
    for (const auto& reading : sensorValues)
    {
        double value = reading.second;
        sum += value;
        if (value < minReading)
        {
            minReading = value;
        }
        if (value > maxReading)
        {
            maxReading = value;
        }
    }
    m_sensorData.averageReading = sensorValues.empty() ? 0.0 : sum / sensorValues.size();
    m_sensorData.minReading = sensorValues.empty() ? 0.0 : minReading;
    m_sensorData.maxReading = sensorValues.empty() ? 0.0 : maxReading;
    return m_sensorData;
}