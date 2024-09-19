#ifndef SENSOR_H
#define SENSOR_H

#include <iostream>
#include <chrono>
#include <ctime>

class Sensor {
public:
    Sensor();
    virtual ~Sensor();

    virtual void reportSensorReading(float reading) = 0;
    virtual std::pair<float, long long> getSensorReading() = 0;
    void displayData();

protected:
    float sensorReading;
    long long readingTime;
};

#endif // SENSOR_H