#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor();
    ~TemperatureSensor() override;

    void reportSensorReading(float reading) override;
    double getSensorReading() override;
    void run(mqtt::async_client& client) override;
};

#endif // TEMPERATURESENSOR_H
