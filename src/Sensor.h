#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <random>
#include <thread>
#include <mqtt/async_client.h>

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
    virtual std::string getSensorData() = 0;
    virtual std::string getSpecificSensorData(const std::string& sensorType) = 0;
    virtual std::string generateSensorId() const = 0;
    std::string getSensorId() const { return sensorId; }

protected:
    void publishReading(mqtt::async_client& client, const std::string& topic, double value);
    std::string sensorId;
    size_t maxSensorValuesSize;

};

#endif // SENSOR_H