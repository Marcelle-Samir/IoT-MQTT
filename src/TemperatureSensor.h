#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor();
    ~TemperatureSensor() override;

    double getSensorReading() override;
    double generateReading() override;
    void run(mqtt::async_client& client) override;
};

#endif // TEMPERATURESENSOR_H
