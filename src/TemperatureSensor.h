#ifndef TEMPERATURESENSOR_H
#define TEMPERATURESENSOR_H

#include "Sensor.h"

class TemperatureSensor : public Sensor
{
public:
    TemperatureSensor();
    ~TemperatureSensor() override;

    void reportSensorReading(float reading) override;
    std::pair<float, long long> getSensorReading() override;
};

#endif // TEMPERATURESENSOR_H
