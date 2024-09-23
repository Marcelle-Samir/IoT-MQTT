#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <chrono>
#include <ctime>
#include <mqtt/async_client.h>

class Sensor
{
public:
    Sensor();
    virtual ~Sensor();

    virtual void reportSensorReading(float reading) = 0;
    virtual double getSensorReading() = 0;
    void displayData();
    virtual void run(mqtt::async_client& client) = 0;

protected:
    void publishReading(mqtt::async_client& client, const std::string& topic, double value);

private:
    float sensorReading;
    long long readingTime;
};

#endif // SENSOR_H