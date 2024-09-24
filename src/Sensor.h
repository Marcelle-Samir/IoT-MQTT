#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>
#include <mqtt/async_client.h>
#include "SensorData.h"

class Sensor
{
public:
    virtual ~Sensor();
    Sensor(size_t maxReadingSize, const std::string& id)
        : maxSensorValuesSize(maxReadingSize), sensorId(id) {}
    virtual double getSensorReading() = 0;
    virtual double generateReading() = 0;
    virtual void run(mqtt::async_client& client) = 0;
    virtual void storeValue(double reading) = 0;
    std::string getSpecificSensorData(const std::string& sensorType);
    virtual std::string generateSensorId() const = 0;
    std::string getSensorId() const { return sensorId; }
    SensorData calculateSensorData();

protected:
    void publishReading(mqtt::async_client& client, const std::string& topic, double value);
    std::string sensorId;
    size_t maxSensorValuesSize;
    std::mutex sensorMutex;
    std::deque<std::pair<std::string, double>> sensorValues;

};

#endif // SENSOR_H